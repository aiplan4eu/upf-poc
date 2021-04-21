#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <chrono>

namespace py = pybind11;

struct heuristic
{
  heuristic(size_t size_, py::object goal_) : size(size_), goal(goal_) {}

  double compute(py::object state)
  {
    auto intersection = state.attr("__and__")(goal);
    return size - py::len(intersection);
  }

  size_t size;
  py::object goal;
};

PYBIND11_EMBEDDED_MODULE(my_heuristic, m) {
    py::class_<heuristic>(m, "Heuristic")
      .def(py::init<size_t, py::object>())
      .def("compute", &heuristic::compute);
}

py::object create_action(py::module_ upf, std::string name, std::vector<std::string> pre,
                         std::vector<std::string> pos, std::vector<std::string> neg)
{
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
  return upf_action(py::str(name), preconditions, positive_effect, negative_effect);
}

py::object generate_problem(py::module_ upf, size_t size)
{
  auto upf_problem = upf.attr("Problem");

  auto actions = py::list();
  auto goal = py::set();

  auto a1 = create_action(upf, "mk_y", {"x"}, {"y"}, {"x"});
  actions.attr("append")(a1);
  auto a2 = create_action(upf, "reset_x", {}, {"x"}, {});
  actions.attr("append")(a2);

  for (size_t i=0; i<size; i++) {
    std::string name = "v" + std::to_string(i);
    goal.attr("add")(py::str(name));
    auto a = create_action(upf, "mk_"+name, {"y"}, {name}, {"y"});
    actions.attr("append")(a);
  }

  auto init = py::set(py::str("x"));
  return upf_problem(actions, init, goal);
}

int main() {
  py::scoped_interpreter guard{};

  auto upf = py::module::import("upf");

  size_t size = 14;

  auto problem = generate_problem(upf, size);

  std::vector<std::string> planners = {"cppplanner_upf", "pyplanner_upf", "jplanner_upf"};

  for (auto& planner_name : planners) {
    auto upf_planner = upf.attr("LinkedSolver")(planner_name);

    {
      auto clock = std::chrono::steady_clock::now();
      auto plan = upf_planner.attr("solve")(problem);
      auto now = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      py::print(plan);
      std::cout << planner_name << " with no heuristic: " << duration.count() << "ms" << std::endl;
    }

    {
      auto my_heuristic = py::module::import("my_heuristic");
      auto clock = std::chrono::steady_clock::now();
      auto plan = upf_planner.attr("solve")(problem, my_heuristic.attr("Heuristic")(size, problem.attr("goal")).attr("compute"));
      auto now = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      py::print(plan);
      std::cout << planner_name << " with heuristic: " << duration.count() << "ms" << std::endl;
    }
  }

  return 0;
}
