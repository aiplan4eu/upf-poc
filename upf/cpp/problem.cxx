#include "problem.hxx"

namespace upf
{

  Problem::Problem(const std::vector<Action>& actions,
                   const std::set<std::string>& init,
                   const std::set<std::string>& goal)
  {
    actions_ = actions;
    init_ = init;
    goal_ = goal;
  }

  const std::vector<Action>& Problem::actions() const
  {
    return actions_;
  }

  const std::set<std::string>& Problem::init() const
  {
    return init_;
  }

  const std::set<std::string>& Problem::goal() const
  {
    return goal_;
  }

}
