#include "jplanner_upf.hxx"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <thread>

#include "JPlannerUPF.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using eu::aiplan4eu::jplanner::grpc::ActionMessage;
using eu::aiplan4eu::jplanner::grpc::ProblemMessage;
using eu::aiplan4eu::jplanner::grpc::ProblemOrHAnswer;
using eu::aiplan4eu::jplanner::grpc::PlanMessage;
using eu::aiplan4eu::jplanner::grpc::PlanOrHRequest;
using eu::aiplan4eu::jplanner::grpc::JPlannerUPF;

ActionMessage MakeActionMessage(const upf::Action& action)
{
  ActionMessage action_message;
  action_message.set_name(action.name());
  for (auto& p : action.precondition()) {
    action_message.add_precondition(p);
  }
  for (auto& p : action.positive_effect()) {
    action_message.add_positiveeffect(p);
  }
  for (auto& p : action.negative_effect()) {
    action_message.add_negativeeffect(p);
  }
  return action_message;
}

ProblemMessage MakeProblemMessage(const upf::Problem& problem)
{
  ProblemMessage problem_message;
  for (auto& i : problem.init()) {
    problem_message.add_init(i);
  }
  for (auto& g : problem.goal()) {
    problem_message.add_goal(g);
  }
  for (auto& a : problem.actions()) {
    problem_message.add_actions()->CopyFrom(MakeActionMessage(a));
  }
  return problem_message;
}

std::vector<std::string> ConvertPlanMessage(PlanMessage plan_message)
{
  std::vector<std::string> res;
  for (int i=0; i<plan_message.actions_size(); i++) {
    res.push_back(plan_message.actions(i));
  }
  return res;
}

class JPlannerUPFClient {
public:
  JPlannerUPFClient(const std::shared_ptr<Channel> channel)
    : stub_(JPlannerUPF::NewStub(channel)) {}

  std::vector<std::string> solve(upf::Problem& problem)
  {
    ClientContext context;
    PlanMessage plan_message;
    stub_->solve(&context, MakeProblemMessage(problem), &plan_message);
    return ConvertPlanMessage(plan_message);
  }

  std::vector<std::string> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
  {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<ProblemOrHAnswer, PlanOrHRequest> > stream(
        stub_->solveWithHeuristic(&context));

    PlanOrHRequest request;
    ProblemOrHAnswer answer;
    answer.mutable_problem()->CopyFrom(MakeProblemMessage(problem));
    stream->Write(answer);
    while (stream->Read(&request)) {
      if (request.has_plan()) {
        stream->WritesDone();
        return ConvertPlanMessage(request.plan());
      }
      auto state_message = request.statetoevaluate();
      std::set<std::string> state;
      for (int i=0; i<state_message.state_size(); i++) {
        auto p = state_message.state(i);
        state.insert(p);
      }
      ProblemOrHAnswer response;
      response.set_stateevaluation(heuristic(state));
      stream->Write(response);
    }
    stream->WritesDone();

    return {};
  }

private:
  std::unique_ptr<JPlannerUPF::Stub> stub_;
};

std::optional<std::vector<std::string>> solve(upf::Problem& problem)
{
  JPlannerUPFClient client(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));

  return client.solve(problem);
}

std::optional<std::vector<std::string>> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  JPlannerUPFClient client(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));

  return client.solve_with_heuristic(problem, heuristic);
}
