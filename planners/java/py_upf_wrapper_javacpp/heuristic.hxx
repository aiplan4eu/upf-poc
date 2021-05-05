#pragma once

#include <set>
#include <string>


class Heuristic {
public:
  virtual double operator()(const std::set<std::string>&)
  {
    return 0;
  }
  virtual ~Heuristic() {}
};


class Action
{
public:
  Action() {};

  Action(const std::string& name,
         const std::set<std::string>& precondition,
         const std::set<std::string>& positive_effect,
         const std::set<std::string>& negative_effect) :
    name_(name),
    precondition_(precondition),
    positive_effect_(positive_effect),
    negative_effect_(negative_effect)
  {}

  const std::string& name() const
  {
    return name_;
  }

  const std::set<std::string>& precondition() const
  {
    return precondition_;
  }

  const std::set<std::string>& positive_effect() const
  {
    return positive_effect_;
  }

  const std::set<std::string>& negative_effect() const
  {
    return negative_effect_;
  }

private:
  std::string name_;
  std::set<std::string> precondition_;
  std::set<std::string> positive_effect_;
  std::set<std::string> negative_effect_;
};


class Problem
{
public:
  Problem() {};

  Problem(const std::vector<Action>& actions,
          const std::set<std::string>& init,
          const std::set<std::string>& goal):
    actions_(actions), init_(init), goal_(goal)
  {}

  const std::vector<Action>& actions() const
  {
    return actions_;
  }

  const std::set<std::string>& init() const
  {
    return init_;
  }

  const std::set<std::string>& goal() const
  {
    return goal_;
  }

private:
  std::vector<Action> actions_;
  std::set<std::string> init_;
  std::set<std::string> goal_;
};
