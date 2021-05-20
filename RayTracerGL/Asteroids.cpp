#include "Asteroids.h"

extern std::vector<std::shared_ptr<Physics::InputSystem>> gInputSystems;

void asteroids(rt::World& w) {
  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
  //  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0, 0, 1.5);
  //  w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id().translate(0, -2, 0), floor_mat));

  rt::Material cube_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().translate(0, 3, 0), cube_mat)); //.scale(2, 4, 2)

                                                                                                          // TODO: check why weak_ptr is not moving correctly
  w.objects()[0]->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 2.0f, rt::zero_vec, rt::zero_vec, rt::zero_vec, rt::zero_vec, false)); //, rt::Vector(0.2f, -0.2f, 0.2f), rt::zero_vec, rt::Vector(0.12f, 0, 0.25f), rt::zero_vec));
  std::map<int, rt::Vector> linearKeyMap;
  linearKeyMap[GLFW_KEY_W] = rt::Vector(0, 0.25f, 0);
  linearKeyMap[GLFW_KEY_S] = rt::Vector(0, -0.25f, 0);
  gInputSystems.emplace_back(std::make_shared<AsteroidsInputSystem>(w.objects().back().get(), linearKeyMap, w));
  w.objects()[0]->behaviors().push_back(gInputSystems.back());
  std::map<int, rt::Vector> angularKeyMap;
  angularKeyMap[GLFW_KEY_A] = rt::Vector(0, 0, 0.01f);
  angularKeyMap[GLFW_KEY_D] = rt::Vector(0, 0, -0.01f);
  gInputSystems.emplace_back(std::make_shared<Physics::AngularInputSystem>(w.objects().back().get(), angularKeyMap));
  w.objects()[0]->behaviors().push_back(gInputSystems.back());

  rt::Material target_mat(rt::Color(1, 1, 1), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(10, 16, 0), target_mat)); //.scale(2, 4, 2)
  w.objects().back()->behaviors().push_back(std::make_shared<Target>(w, w.objects().back().get(), 2.0f, rt::zero_vec, rt::zero_vec, rt::Vector(1, 0, 0), rt::zero_vec));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(-10, -18, 0), target_mat)); //.scale(2, 4, 2)
  w.objects().back()->behaviors().push_back(std::make_shared<Target>(w, w.objects().back().get(), 2.0f, rt::zero_vec, rt::zero_vec, rt::Vector(1, 0, 0), rt::zero_vec));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(-18, -7, 0), target_mat)); //.scale(2, 4, 2)
  w.objects().back()->behaviors().push_back(std::make_shared<Target>(w, w.objects().back().get(), 2.0f, rt::zero_vec, rt::zero_vec, rt::Vector(1, 0, 0), rt::zero_vec));
}
