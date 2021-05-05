#pragma once

#include <optional>
#include <functional>
#include <string>
#include <vector>

#include "heuristic.hxx"

extern "C"
{

  void init();

  void uninit();

  std::optional<std::vector<std::string>> solve_with_heuristic(Problem& problem, std::function<double(std::set<std::string>)> heuristic);
}
