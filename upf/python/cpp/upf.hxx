#pragma once

#include <string>
#include <set>
#include <vector>
#include <optional>
#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "problem.hxx"

namespace py = pybind11;

namespace upf
{

  class Heuristic {
  public:
    virtual double operator()(const std::set<std::string>& s)
    {
      return 0;
    }

    virtual ~Heuristic() {}
  };

  void init() {
    py::initialize_interpreter();
  }

  void uninit() {
    py::finalize_interpreter();
  }

  std::optional<std::vector<std::string> > solve(std::string planner, Problem problem) {
    auto upf = py::module::import("upf");
    auto upf_planner = upf.attr("LinkedSolver")(planner);
    auto plan = upf_planner.attr("solve")(problem.pyproblem());
    std::vector<std::string> res;
    for (py::handle obj : plan) {
      res.push_back(obj.attr("__str__")().cast<std::string>());
    }
    return res;
  }

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem, Heuristic& h) {
    auto upf = py::module::import("upf");
    auto upf_planner = upf.attr("LinkedSolver")(planner);
    auto plan = upf_planner.attr("solve")(problem.pyproblem(), py::cpp_function(h));
    std::vector<std::string> res;
    for (py::handle obj : plan) {
      res.push_back(obj.attr("__str__")().cast<std::string>());
    }
    return res;
  }

  bool set_contains(const std::set<std::string>& s, const std::string& x) {
    return (s.find(x) != s.end());
  }

}
