#include "RigidBody.h"

#include "Collidable.h"

#include "../RayTracerLib/World.h"

namespace Physics {

  bool RigidBody::action() {
    if (_obj) {
      // Check for collisions
      auto new_vel = _velocity + _acceleration;
      if (rt::magnitude(new_vel) > rt::epsilon) {
        auto xs = _world.intersect(rt::Ray(location(), rt::normalize(new_vel)));

        // Skip objects in the back and objects inside 
        // (if this object is inside another, it's the other's responsibility to apply force on this)
        auto it = xs.begin();
        while (it != xs.end() && (it->time() < 0 || &(it->object()) != _obj)) {
          ++it;
        }

        if (it + 1 != xs.end()) {
          // Distance between front of the object and next object
          auto dist = (it+1)->time() - it->time();

          //std::cout << dist << ", " << rt::magnitude(new_vel) << std::endl;

          // If next step is going to overshoot
          if (dist <= rt::magnitude(new_vel)) {
            auto& collision_obj = (it+1)->object();
            for (std::shared_ptr<Behavior> b : collision_obj.behaviors()) {
              auto other = std::dynamic_pointer_cast<Collidable>(b);
              if (!other) {
                continue;
              }
              // Act as if the collision already happened
              other->collide(*this);
              // But move the object closer to the collided object
              _obj->move(rt::Transform::id().translate(rt::normalize(new_vel) * (dist - rt::epsilon)) * _obj->transform());
            }
          }
        }
      }

      _velocity = _velocity + _acceleration;
      _obj->move(rt::Transform::id().translate(_velocity) * _obj->transform());
      _force = _acceleration;
      _acceleration = rt::zero_vec;

      _aVelocity = _aVelocity + _aAcceleration;
      _obj->move(_obj->transform() * rt::Transform::id().rot_z(_aVelocity.z) * rt::Transform::id().rot_y(_aVelocity.y) * rt::Transform::id().rot_x(_aVelocity.x));
      _aAcceleration = rt::zero_vec;

    }
    return true;
  }

} // namespace Physics