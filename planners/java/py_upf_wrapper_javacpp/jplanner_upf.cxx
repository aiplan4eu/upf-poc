#include <iostream>
#include <cstdlib>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "jplanner_upf.hxx"
#include "jniPlannerWrapper.h"

namespace py = pybind11;


class NewHeuristic : public Heuristic {
public:
  NewHeuristic(std::function<double(std::set<std::string>)>& heuristic) :
    heuristic_(heuristic) {}

  double operator()(const std::set<std::string>& s) override
  {
    return heuristic_(s);
  }

private:
  std::function<double(std::set<std::string>)> heuristic_;
};


void init() {
  if (std::getenv("CLASSPATH")) {
    auto classpath = std::string("-Djava.class.path=")+std::getenv("CLASSPATH");
    const char* a[1] = {classpath.c_str()};
    JavaCPP_init(1, a);
  }
  else {
    JavaCPP_init(0, NULL);
  }
}

void uninit() {
  JavaCPP_uninit();
}

std::optional<std::vector<std::string>> solve_with_heuristic(Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  NewHeuristic h(heuristic);
  auto res = jsolve_with_heuristic(problem, h);
  return res;
}


PYBIND11_MODULE(_jplanner_upf, m) {
    m.doc() = "UPF java->c++->python example";

    py::class_<Action>(m, "Action")
      .def(py::init<const std::string&,
                    const std::set<std::string>&,
                    const std::set<std::string>&,
                    const std::set<std::string>&>())
      .def_property_readonly("name", &Action::name)
      .def_property_readonly("precondition", &Action::name)
      .def_property_readonly("positive_effect", &Action::name)
      .def_property_readonly("negative_effect", &Action::name);

    py::class_<Problem>(m, "Problem")
      .def(py::init<const std::vector<Action>&,
                    const std::set<std::string>&,
                    const std::set<std::string>&>())
      .def_property_readonly("actions", &Problem::actions)
      .def_property_readonly("init", &Problem::init)
      .def_property_readonly("goal", &Problem::goal);

    m.def("solve",
          &jsolve,
          "Solves a problem without heuristic");
    m.def("solve_with_heuristic",
          &solve_with_heuristic,
          "Solves a problem with heuristic");

    m.def("init",
          &init,
          "Initializes JVM");

    m.def("uninit",
          &uninit,
          "Un-Initializes JVM");

}
