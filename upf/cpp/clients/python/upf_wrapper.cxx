#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "upf.hxx"

namespace py = pybind11;

PYBIND11_MODULE(_pywrapper_upf, m) {
    m.doc() = "UPF c++->python example";

    py::class_<upf::Action>(m, "Action")
      .def(py::init<const std::string&,
                    const std::set<std::string>&,
                    const std::set<std::string>&,
                    const std::set<std::string>&>())
      .def_property_readonly("name", &upf::Action::name)
      .def_property_readonly("precondition", &upf::Action::name)
      .def_property_readonly("positive_effect", &upf::Action::name)
      .def_property_readonly("negative_effect", &upf::Action::name);

    py::class_<upf::Problem>(m, "Problem")
      .def(py::init<const std::vector<upf::Action>&,
                    const std::set<std::string>&,
                    const std::set<std::string>&>())
      .def_property_readonly("actions", &upf::Problem::actions)
      .def_property_readonly("init", &upf::Problem::init)
      .def_property_readonly("goal", &upf::Problem::goal);

    m.def("solve",
          py::overload_cast<std::string, upf::Problem>(&upf::solve),
          "Solves a problem without heuristic");
    m.def("solve_with_heuristic",
          py::overload_cast<std::string, upf::Problem, std::function<double(std::set<std::string>)>>(&upf::solve),
          "Solves a problem with heuristic");

}
