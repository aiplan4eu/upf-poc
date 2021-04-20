#include "action.hxx"

namespace upf
{

  Action::Action(const std::string& name,
                 const std::set<std::string>& precondition,
                 const std::set<std::string>& positive_effect,
                 const std::set<std::string>& negative_effect)
  {
    name_ = name;
    precondition_ = precondition;
    positive_effect_ = positive_effect;
    negative_effect_ = negative_effect;
  }

  const std::string& Action::name() const
  {
    return name_;
  }

  const std::set<std::string>& Action::precondition() const
  {
    return precondition_;
  }

  const std::set<std::string>& Action::positive_effect() const
  {
    return positive_effect_;
  }

  const std::set<std::string>& Action::negative_effect() const
  {
    return negative_effect_;
  }

  bool Action::operator<(const Action& oth) const
  {
    return name_ < oth.name();
  }

}
