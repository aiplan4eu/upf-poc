#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "UPF.grpc.pb.h"

#include "upf.hxx"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using eu::aiplan4eu::upf::grpc::ActionMessage;
using eu::aiplan4eu::upf::grpc::ProblemMessage;
using eu::aiplan4eu::upf::grpc::ProblemOrHAnswer;
using eu::aiplan4eu::upf::grpc::PlanMessage;
using eu::aiplan4eu::upf::grpc::PlanOrHRequest;
using eu::aiplan4eu::upf::grpc::State;
using eu::aiplan4eu::upf::grpc::UPF;

upf::Action ConvertActionMessage(const ActionMessage& action_message)
{
  std::set<std::string> precondition;
  std::set<std::string> positive_effect;
  std::set<std::string> negative_effect;
  for (int i=0; i<action_message.precondition_size(); i++) {
    precondition.insert(action_message.precondition(i));
  }
  for (int i=0; i<action_message.positiveeffect_size(); i++) {
    positive_effect.insert(action_message.positiveeffect(i));
  }
  for (int i=0; i<action_message.negativeeffect_size(); i++) {
    negative_effect.insert(action_message.negativeeffect(i));
  }
  upf::Action action(action_message.name(), precondition, positive_effect, negative_effect);
  return action;
}

upf::Problem ConvertProblemMessage(const ProblemMessage& problem_message)
{
  std::vector<upf::Action> actions;
  std::set<std::string> init;
  std::set<std::string> goal;
  for (int i=0; i<problem_message.init_size(); i++) {
    init.insert(problem_message.init(i));
  }
  for (int i=0; i<problem_message.goal_size(); i++) {
    goal.insert(problem_message.goal(i));
  }
  for (int i=0; i<problem_message.actions_size(); i++) {
    actions.push_back(ConvertActionMessage(problem_message.actions(i)));
  }
  upf::Problem problem(actions, init, goal);
  return problem;
}

class UPFImpl final : public UPF::Service {
 public:

  Status solve(ServerContext* context, const ProblemMessage* request, PlanMessage* response)
  {
    auto p = request->planner();
    if (planners_.find(p) == planners_.end()) {
      upf::init(p);
      planners_.insert(p);
    }
    if (auto plan = upf::solve(p, ConvertProblemMessage(*request))) {
      for (auto& s : *plan) {
        response->add_actions(s);
      }
    }
    return Status::OK;
  }

  Status solveWithHeuristic(ServerContext* context,
                            ServerReaderWriter<PlanOrHRequest, ProblemOrHAnswer>* stream)
  {
    ProblemOrHAnswer problem;
    stream->Read(&problem);
    auto h = [stream](std::set<std::string> state){
               State state_grpc;
               for (auto& s : state) {
                 state_grpc.add_state(s);
               }
               PlanOrHRequest request;
               request.mutable_statetoevaluate()->CopyFrom(state_grpc);
               stream->Write(request);
               ProblemOrHAnswer response;
               stream->Read(&response);
               return response.stateevaluation();
             };
    auto p = problem.problem().planner();
    if (planners_.find(p) == planners_.end()) {
      upf::init(p);
      planners_.insert(p);
    }
    if (auto plan = upf::solve(p, ConvertProblemMessage(problem.problem()), h)) {
      PlanOrHRequest response;
      PlanMessage message;
      for (auto& s : *plan) {
        message.add_actions(s);
      }
      response.mutable_plan()->CopyFrom(message);
      stream->Write(response);
    }
    return Status::OK;
  }

  ~UPFImpl()
  {
    for (auto& p : planners_) {
      upf::uninit(p);
    }
  }

private:
  std::set<std::string> planners_;

};


int main() {
  std::string server_address("localhost:50052");
  UPFImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}
