#include<algorithm>
#include<memory>
#include<unordered_set>

#include "planner.hxx"


namespace cppplanner
{
  using EType = std::tuple<double, std::shared_ptr<std::set<std::string>>, std::vector<std::string>>;

  template <class T> inline void hash_combine(std::size_t& seed, T const& v)
  {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  struct set_string_hash
  {
    std::size_t operator()(std::shared_ptr<std::set<std::string>> const& obj) const
    {
      std::size_t res = 0;
      for (auto& x : *obj) {
        hash_combine(res, x);
      }
      return res;
    }
  };

  struct set_string_eq
  {
    bool operator()(std::shared_ptr<std::set<std::string>> const& obj1, std::shared_ptr<std::set<std::string>> const& obj2) const
    {
      return *obj1 == *obj2;
    }
  };



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


  template<typename T>
  static bool is_subset(const std::set<T>& a, const std::set<T>& b)
  {
    for (auto& x : a) {
      if (b.find(x) == b.end()) {
        return false;
      }
    }
    return true;
  }


  static inline std::shared_ptr<std::set<std::string>> action_apply(const Action& a, const std::set<std::string>& state)
  {
    auto child = std::make_shared<std::set<std::string>>(state);
    for (auto& x : a.negative_effect()) {
      child->erase(x);
    }
    for (auto& x : a.positive_effect()) {
      child->insert(x);
    }
    return child;
  }

  std::optional<std::vector<std::string>> solve(Problem problem, std::function<double(std::set<std::string>)> heuristic)
  {
    std::priority_queue<EType, std::vector<EType>, std::greater<EType>> open_list;
    double initf = heuristic(problem.init());
    open_list.emplace(initf, std::make_shared<std::set<std::string>>(problem.init()), std::vector<std::string>{});
    std::unordered_set<std::shared_ptr<std::set<std::string>>, set_string_hash, set_string_eq> closed_list;
    while (!open_list.empty()) {
      auto current = open_list.top();
      open_list.pop();
      auto state = std::get<1>(current);
      if (closed_list.find(state) == closed_list.end()) {
        closed_list.insert(state);
        if (is_subset(problem.goal(), *state)) {
          return std::get<2>(current);
        }
        else {
          for (auto& a : problem.actions()) {
            if (is_subset(a.precondition(), *state)) {
              auto child = action_apply(a, *state);
              if (closed_list.find(child) == closed_list.end()) {
                double f = std::get<2>(current).size() + 1 + heuristic(*child);
                auto new_path = std::get<2>(current);
                new_path.push_back(a.name());
                open_list.emplace(f, child, new_path);
              }
            }
          }
        }
      }
    }
    return std::nullopt;
  }

  std::optional<std::vector<std::string>> solve(Problem problem)
  {
    return solve(problem, [](const std::set<std::string>& state){ return 0; });
  }

}
