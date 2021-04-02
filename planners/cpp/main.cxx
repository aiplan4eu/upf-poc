#include <iostream>

#include "planner.hxx"

using namespace cppplanner;

int main()
{
  Action a("a", {"x"}, {"y"}, {"x"});
  Action b("b", {"y"}, {"z"}, {"y"});
  Action c("c", {"y"}, {"w"}, {"y"});
  Action d("d", {}, {"x"}, {});

  Problem problem({a,b,c,d}, {"x"}, {"z", "w"});

  auto plan = solve(problem, [](const std::set<std::string>& state) {
                               if (state.find("z") != state.end()) return 1.0;
                               if (state.find("w") != state.end()) return 1.0;
                               return 10.0;
                             });

  if (plan.has_value()) {
    for (auto& a : plan.value()) {
      std::cout << a << " ";
    }
    std::cout << std::endl;
  }
  else {
    std::cout << "No Plan" << std::endl;
  }

  return 0;
}
