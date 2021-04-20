#include "pyplanner_upf.hxx"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;


std::optional<std::vector<std::string>> _solve(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  auto pyplanner = py::module::import("pyplanner");
  auto frozenset = py::module::import("builtins").attr("frozenset");

  auto planner_problem = pyplanner.attr("Problem");
  auto planner_action = pyplanner.attr("Action");
  auto actions = py::list();
  for (auto& a : problem.actions()) {
    auto preconditions = py::set();
    for (auto& p : a.precondition()) {
      preconditions.attr("add")(py::str(p));
    }
    auto positive_effect = py::set();
    for (auto& p : a.positive_effect()) {
      positive_effect.attr("add")(py::str(p));
    }
    auto negative_effect = py::set();
    for (auto& p : a.negative_effect()) {
      negative_effect.attr("add")(py::str(p));
    }
    auto new_a = planner_action(py::str(a.name()), frozenset(preconditions),
                                frozenset(positive_effect), frozenset(negative_effect));
    actions.attr("append")(new_a);
  }
  auto init = py::set();
  for (auto& p : problem.init()) {
    init.attr("add")(py::str(p));
  }
  auto goal = py::set();
  for (auto& p : problem.goal()) {
    goal.attr("add")(py::str(p));
  }
  auto p = planner_problem(actions, frozenset(init), frozenset(goal));

  auto solver = pyplanner.attr("Solver")(heuristic);

  auto pyplan = py::list(solver.attr("solve")(p));
  std::vector<std::string> res;
  for (py::handle obj : pyplan) {
    res.push_back(obj.attr("__str__")().cast<std::string>());
  }
  return res;
}


std::optional<std::vector<std::string>> solve(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  try {
    py::scoped_interpreter guard{};
    return _solve(problem, heuristic);
  } catch (const std::runtime_error& e) {
    return _solve(problem, heuristic);
  }
}
