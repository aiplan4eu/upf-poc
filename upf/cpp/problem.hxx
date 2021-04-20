#pragma once

#include <string>
#include <set>
#include <vector>

#include "action.hxx"

namespace upf
{

  class Problem
  {
  public:
    Problem(const std::vector<Action>& actions,
            const std::set<std::string>& init,
            const std::set<std::string>& goal);

    const std::vector<Action>& actions() const;

    const std::set<std::string>& init() const;

    const std::set<std::string>& goal() const;

  private:
    std::vector<Action> actions_;
    std::set<std::string> init_;
    std::set<std::string> goal_;
  };

}
