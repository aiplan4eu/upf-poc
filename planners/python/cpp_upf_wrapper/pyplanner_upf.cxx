#include "pyplanner_upf.hxx"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;


void init() {}

void uninit() {}

py::object convert_problem(upf::Problem& problem)
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
  return p;
}

std::vector<std::string> convert_plan(py::list pyplan)
{
  std::vector<std::string> res;
  for (py::handle obj : pyplan) {
    res.push_back(obj.attr("__str__")().cast<std::string>());
  }
  return res;
}

std::optional<std::vector<std::string>> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  try {
    py::scoped_interpreter guard{};
    auto p = convert_problem(problem);
    auto pyplanner = py::module::import("pyplanner");
    auto solver = pyplanner.attr("Solver")(heuristic);
    auto pyplan = py::list(solver.attr("solve")(p));
    return convert_plan(pyplan);
  } catch (const std::runtime_error& e) {
    auto p = convert_problem(problem);
    auto pyplanner = py::module::import("pyplanner");
    auto solver = pyplanner.attr("Solver")(heuristic);
    auto pyplan = py::list(solver.attr("solve")(p));
    return convert_plan(pyplan);
  }
}

std::optional<std::vector<std::string>> solve(upf::Problem& problem)
{
  try {
    py::scoped_interpreter guard{};
    auto p = convert_problem(problem);
    auto pyplanner = py::module::import("pyplanner");
    auto solver = pyplanner.attr("Solver")();
    auto pyplan = py::list(solver.attr("solve")(p));
    return convert_plan(pyplan);
  } catch (const std::runtime_error& e) {
    auto p = convert_problem(problem);
    auto pyplanner = py::module::import("pyplanner");
    auto solver = pyplanner.attr("Solver")();
    auto pyplan = py::list(solver.attr("solve")(p));
    return convert_plan(pyplan);
  }
}
