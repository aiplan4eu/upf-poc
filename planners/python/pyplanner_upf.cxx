#include "pyplanner_upf.hxx"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;

struct heuristic_class
{
  heuristic_class(std::function<double(std::set<std::string>)> heuristic_) : heuristic_fun(heuristic_) {}

  double compute(py::object state)
  {
    std::set<std::string> set;
    for (py::handle obj : state) {
      set.insert(obj.attr("__str__")().cast<std::string>());
    }
    return heuristic_fun(set);
  }

  std::function<double(std::set<std::string>)> heuristic_fun;
};

PYBIND11_EMBEDDED_MODULE(my_heuristic, m) {
    py::class_<heuristic_class>(m, "Heuristic")
      .def(py::init<std::function<double(std::set<std::string>)> >())
      .def("__call__", &heuristic_class::compute);
}

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

  auto my_heuristic = py::module::import("my_heuristic");
  auto solver = pyplanner.attr("Solver")(my_heuristic.attr("Heuristic")(heuristic));

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
