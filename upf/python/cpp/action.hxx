#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <string>
#include <set>

namespace py = pybind11;

namespace upf
{

  class Action
  {
  public:

    Action() {}

    Action(const std::string& name,
           const std::set<std::string>& pre,
           const std::set<std::string>& pos,
           const std::set<std::string>& neg)
    {
      auto upf = py::module::import("upf");
      auto upf_action = upf.attr("Action");
      auto preconditions = py::set();
      for (auto& p : pre) {
        preconditions.attr("add")(py::str(p));
      }
      auto positive_effect = py::set();
      for (auto& p : pos) {
        positive_effect.attr("add")(py::str(p));
      }
      auto negative_effect = py::set();
      for (auto& p : neg) {
        negative_effect.attr("add")(py::str(p));
      }
      pyaction_ = upf_action(py::str(name), preconditions, positive_effect, negative_effect);
    }

    py::object pyaction() const { return pyaction_; }

  private:
    py::object pyaction_;
  };

}
