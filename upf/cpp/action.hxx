#pragma once

#include <string>
#include <set>

namespace upf
{

  class Action
  {
  public:
    Action() {}

    Action(const std::string& name,
           const std::set<std::string>& precondition,
           const std::set<std::string>& positive_effect,
           const std::set<std::string>& negative_effect);

    const std::string& name() const;

    const std::set<std::string>& precondition() const;

    const std::set<std::string>& positive_effect() const;

    const std::set<std::string>& negative_effect() const;

    bool operator<(const Action& oth) const;

  private:
    std::string name_;
    std::set<std::string> precondition_;
    std::set<std::string> positive_effect_;
    std::set<std::string> negative_effect_;
  };

}
