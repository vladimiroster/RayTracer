#pragma once

namespace Physics {
  class Behavior {
  public:
    virtual void setup() = 0;
    // Returns whether object is still alive
    virtual bool action() = 0;
  };
} // namespace Physics
