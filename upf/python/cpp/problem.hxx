#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <string>
#include <set>
#include <vector>

#include "action.hxx"

namespace py = pybind11;

namespace upf
{

  class Problem
  {
  public:
    Problem(const std::vector<Action>& actions,
            const std::set<std::string>& init,
            const std::set<std::string>& goal)
    {
      auto upf = py::module::import("upf");
      auto upf_problem = upf.attr("Problem");
      auto pyactions = py::list();
      for (auto& a : actions) {
        pyactions.attr("append")(a.pyaction());
      }
      auto pyinit = py::set();
      for (auto& i : init) {
        pyinit.attr("add")(py::str(i));
      }
      auto pygoal = py::set();
      for (auto& g : goal) {
        pygoal.attr("add")(py::str(g));
      }
      pyproblem_ = upf_problem(pyactions, pyinit, pygoal);
    }

    py::object pyproblem() const { return pyproblem_; }

  private:
    py::object pyproblem_;
  };

}
