#pragma once

namespace Physics {
  class Behavior {
  public:
    virtual void setup() = 0;
    virtual void action() = 0;
  };
} // namespace Physics
