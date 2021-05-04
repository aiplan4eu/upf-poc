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
