#pragma once

#include <optional>
#include <functional>
#include <string>
#include <vector>

#include "problem.hxx"

extern "C"
{

  std::optional<std::vector<std::string>> solve(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic);

}
