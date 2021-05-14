#pragma once

#include <memory>
#include <map>
#include <set>

#include "RigidBody.h"

#include "../RayTracerLib/Object.h"

namespace rt = RayTracer;

namespace Physics {

  class InputSystem : public Behavior {
  public:
    InputSystem(rt::Object* obj, const std::map<int, rt::Vector>& keyMap) : _keyMap(keyMap) {
      // TODO: utility for extracting rigidbody from object
      for (std::shared_ptr<Behavior> b : obj->behaviors()) {
        std::shared_ptr<RigidBody> rb = std::dynamic_pointer_cast<RigidBody>(b);
        if (rb) {
          _rb = rb;
          break;
        }
      }
    }

    virtual void setup() override {}

    virtual void action() override {
      if (_keys.size() > 0) {
        local_action();
      }
    }

    void keyPress(int key) {
      _keys.insert(key);
    }

    void keyRelease(int key) {
      _keys.erase(key);
    }

  protected:
    virtual void local_action() = 0;

    std::shared_ptr<RigidBody> _rb;
    std::map<int, rt::Vector> _keyMap;

    bool _keyPressed = false;
    std::set<int> _keys;
  };

  class LinearInputSystem : public InputSystem {
  public:
    LinearInputSystem(rt::Object* obj, const std::map<int, rt::Vector>& keyMap) :
      InputSystem(obj, keyMap) {};

  private:
    virtual void local_action() override {
      for (auto k : _keys) {
        auto it = _keyMap.find(k);
        if (it != _keyMap.end())
        _rb->apply_local_force(it->second);
      }
    }
  };

  class AngularInputSystem : public InputSystem {
  public:
    AngularInputSystem(rt::Object* obj, const std::map<int, rt::Vector>& keyMap) :
      InputSystem(obj, keyMap) {};
  private:
    virtual void local_action() override {
      for (auto k : _keys) {
        auto it = _keyMap.find(k);
        if (it != _keyMap.end())
          _rb->apply_local_angular_force(it->second);
      }
    }
  };

} // namespace Physics
