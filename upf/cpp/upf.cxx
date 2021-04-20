#include "upf.hxx"

#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>

namespace upf
{

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem, std::function<double(std::set<std::string>)> heuristic)
  {
    void* handle = dlopen(planner.c_str(), RTLD_LAZY);
    if (handle == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return std::nullopt;
    }
    void* f = dlsym(handle, "solve");
    if (f == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return std::nullopt;
    }
    std::optional<std::vector<std::string>>
      (*plannersolve)(Problem&, std::function<double(std::set<std::string>)>);
    plannersolve = reinterpret_cast<std::optional<std::vector<std::string>> (*) (Problem&, std::function<double(std::set<std::string>)>)>(f);
    auto res = plannersolve(problem, heuristic);
    dlclose(handle);
    return res;
  }

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem)
  {
    return solve(planner, problem, [](const std::set<std::string>& state){ return 0; });
  }

}
