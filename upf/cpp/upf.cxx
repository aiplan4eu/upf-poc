#include "upf.hxx"

#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>

namespace upf
{

  void init(std::string planner)
  {
    void* handle = dlopen(planner.c_str(), RTLD_LAZY);
    if (handle == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return;
    }
    void* f = dlsym(handle, "init");
    if (f == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return;
    }
    void (*plannerinit)();
    plannerinit = reinterpret_cast<void (*) ()>(f);
    plannerinit();
    dlclose(handle);
  }

  void uninit(std::string planner)
  {
    void* handle = dlopen(planner.c_str(), RTLD_LAZY);
    if (handle == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return;
    }
    void* f = dlsym(handle, "uninit");
    if (f == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return;
    }
    void (*planneruninit)();
    planneruninit = reinterpret_cast<void (*) ()>(f);
    planneruninit();
    dlclose(handle);
  }

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem, std::function<double(std::set<std::string>)> heuristic)
  {
    void* handle = dlopen(planner.c_str(), RTLD_LAZY);
    if (handle == 0) {
      std::cerr << "Error: " << dlerror() << std::endl;
      return std::nullopt;
    }
    void* f = dlsym(handle, "solve_with_heuristic");
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
    std::optional<std::vector<std::string>> (*plannersolve)(Problem&);
    plannersolve = reinterpret_cast<std::optional<std::vector<std::string>> (*) (Problem&)>(f);
    auto res = plannersolve(problem);
    dlclose(handle);
    return res;
  }

}
