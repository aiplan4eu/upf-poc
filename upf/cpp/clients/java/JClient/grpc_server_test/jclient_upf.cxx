#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "JClientUPF.grpc.pb.h"

#include "upf.hxx"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using eu::aiplan4eu::jclient::grpc::ActionMessage;
using eu::aiplan4eu::jclient::grpc::ProblemMessage;
using eu::aiplan4eu::jclient::grpc::ProblemOrHAnswer;
using eu::aiplan4eu::jclient::grpc::PlanMessage;
using eu::aiplan4eu::jclient::grpc::PlanOrHRequest;
using eu::aiplan4eu::jclient::grpc::JClientUPF;

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

class JClientUPFImpl final : public JClientUPF::Service {
 public:

  Status solve(ServerContext* context, const ProblemMessage* request, PlanMessage* response)
  {
    if (auto plan = upf::solve(request->planner(), ConvertProblemMessage(*request))) {
      for (auto& s : *plan) {
        response->add_actions(s);
      }
    }
    return Status::OK;
  }

  Status solveWithHeuristic(ServerContext* context,
                            ServerReaderWriter<PlanOrHRequest, ProblemOrHAnswer>* stream)
  {

    return Status::OK;
  }

};


int main() {
  std::string server_address("localhost:50052");
  JClientUPFImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

  return 0;
}
