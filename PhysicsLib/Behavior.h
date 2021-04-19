#pragma once

namespace Physics {
  class Behavior {
  public:
    virtual void Setup() = 0;
    virtual void Action() = 0;
  };
} // namespace Physics
