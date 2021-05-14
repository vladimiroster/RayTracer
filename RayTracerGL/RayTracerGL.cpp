#include <GLFW/glfw3.h>

#include <chrono>
using namespace std::chrono_literals;
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Cube.h"
#include "../RayTracerLib/Plane.h"
#include "../RayTracerLib/Camera.h"
#include "../RayTracerLib/Profiler.h"

#include "../PhysicsLib/RandomWalker.h"
#include "../PhysicsLib/Distribution.h"
#include "../PhysicsLib/BouncingObject.h"
#include "../PhysicsLib/Mover.h"
#include "../PhysicsLib/RigidBody.h"
#include "../PhysicsLib/Engine.h"
#include "../PhysicsLib/Liquid.h"
#include "../PhysicsLib/InputSystem.h"

namespace rt = RayTracer;

// TODO: find a better place for these (and design the GL app better in general)
rt::Point FROM(0, 1.5f, -50);
rt::Point TO(0, 1, 0);
rt::Vector UP(0, 1, 0);
// Away is +z
auto res = rt::Camera::RES_640X480;
float FOV = 3.14159f / 3; //3.14159f / 3.0f;
float MOVE_DELTA = 0.1f;
std::unique_ptr<rt::Canvas> canvas;

bool gPlayMode = false;
std::vector<std::shared_ptr<Physics::InputSystem>> gInputSystems;

void process_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {
    for (auto is : gInputSystems) {
      is->keyPress(key);
    }
    switch (key) {
    case GLFW_KEY_SPACE:
      gPlayMode = !gPlayMode;
      break;
    case GLFW_KEY_KP_SUBTRACT:
      FOV += MOVE_DELTA;
      break;
    case GLFW_KEY_KP_ADD:
      FOV -= MOVE_DELTA;
      break;
    case GLFW_KEY_LEFT:
      FROM = rt::Transform::id().rot_y(MOVE_DELTA) * FROM;
      break;
    case GLFW_KEY_RIGHT:
      FROM = rt::Transform::id().rot_y(-MOVE_DELTA) * FROM;
      break;
    case GLFW_KEY_UP:
      FROM = rt::Transform::id().rot_x(MOVE_DELTA) * FROM;
      break;
    case GLFW_KEY_DOWN:
      FROM = rt::Transform::id().rot_x(-MOVE_DELTA) * FROM;
      break;
    case GLFW_KEY_HOME:
      UP = rt::Transform::id().rot_z(3.14159f / 4.0f) * UP;
      break;
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_S:
      // Note: this needs to be last case because of c++ ctor issues
      std::stringstream strm;
      auto id = std::chrono::system_clock::now().time_since_epoch().count();
      strm << "c:\\temp\\RayTracer\\rt" << std::setfill('0') << std::setw(10) << id << ".ppm";
      std::cout << "Writing frame " << strm.str() << '\n';
      std::ofstream of(strm.str());
      of << *canvas;
      std::cout << "Done writing frame\n";
      break;
    }
  }
  else if (action == GLFW_RELEASE) {
    for (auto is : gInputSystems) {
      is->keyRelease(key);
    }
  }
}

void load_world_1(rt::World& w) {
  auto plane_scale = rt::Transform::id().scale(10, 0.1f, 10);
  rt::Material floor_mat(rt::Color(1, 0.9f, 0.9f), 0.1f, 0.9f, 0, 0, 0, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(plane_scale, floor_mat));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(-0.7853975f /* -pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(0.7853975f /* pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  rt::Material middle_mat(rt::Color(0.1f, 1, 0.5f), 0.1f, 0.7f, 0.3f, 200, 0, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-0.5f, 1, 0.5f), middle_mat));
  rt::Material right_mat(rt::Color(0.5f, 1, 0.1f), 0.1f, 0.7f, 0.3f, 200, 0, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(1.5f, 0.5f, -0.5f), right_mat));
  rt::Material left_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 0, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(-1.5f, 0.33f, -0.75f), left_mat));

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
}

// TODO: https://www.w3.org/TR/2018/REC-css-color-3-20180619/#svg-color
// Mirror material: Color(0.1, 0.1, 0.1), ambient: 0.1, diffuse: 0.7, reflective: 0.9, specular: 0.3, shininess: 200.0)

void load_world_2(rt::World& w) {
  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0.5, 0.5, 1.5,
    std::make_shared<rt::CheckersPattern>(
      std::make_shared<rt::StripePattern>(std::make_shared<rt::SolidPattern>(rt::black), std::make_shared<rt::SolidPattern>(rt::white)), 
      std::make_shared<rt::StripePattern>(std::make_shared<rt::SolidPattern>(rt::Color(1, 0, 0)), std::make_shared<rt::SolidPattern>(rt::Color(0, 0, 1)), rt::Transform::id().rot_x(3.14159f/2))
    ));
  w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id(), floor_mat));
  rt::Material middle_mat(rt::Color(0.1f, 1, 0.5f), 0.1f, 0.7f, 0.3f, 200, 0.5f, 0.5f, rt::glass); //, std::make_shared<rt::CheckersPattern>(std::make_shared<rt::SolidPattern>(rt::Color(1, 1, 0.1f)), std::make_shared<rt::SolidPattern>(rt::Color(0, 0, 0.1f)), rt::Transform::id().scale(0.5f, 0.5f, 0.5f)));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-0.5f, 1, 0.5f), middle_mat));
  rt::Material right_mat(rt::Color(1, 0, 1), 0.1f, 0.7f, 0.3f, 200, 1, 1, rt::diamond); //, std::make_shared<rt::StripePattern>(std::make_shared<rt::SolidPattern>(rt::Color(0.5f, 1, 0.1f)), std::make_shared<rt::SolidPattern>(rt::Color(0.5f, 0, 0.1f)), rt::Transform::id().scale(0.25f, 0.25f, 0.25f).rot_z(3.14159f / 3.0f)));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(1.5f, 0, -0.5f), right_mat));
  rt::Material left_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(-1.5f, 0.33f, -0.75f), left_mat));
  //w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().translate(0, 1.5f, 1.5f).translate(-1.5f, 0.33f, -0.75f), middle_mat));

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
  //w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(0.5, 0.5, 0), rt::Point(-5, 5, 5)));
}

void load_world_3(rt::World& w) {
  //w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));

  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0.5, 0.5, 1.5);
  //w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id(), floor_mat));

  // Table
  rt::Material table_mat(rt::Color(1, 1, 1), 0.1f, 0.7f, 0.5f, 200, 0, 0, 100); //rt::glass);
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().translate(0, 8, 0).scale(3, 0.1f, 1), table_mat)); // Table top
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(0.1f, 0.8f, 0.1f).translate(1.8f, -0.05f, 0.8f), table_mat)); // Back right leg
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(0.1f, 0.8f, 0.1f).translate(1.8f, -0.05f, -0.8f), table_mat)); // Front right leg
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(0.1f, 0.8f, 0.1f).translate(-1.8f, -0.05f, 0.8f), table_mat)); // Back left leg
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(0.1f, 0.8f, 0.1f).translate(-1.8f, -0.05f, -0.8f), table_mat)); // Front left leg

  // Lamp
  rt::Material lamp_mat(rt::Color(1, 1, 0), 0.1f, 0.7f, 0.5f, 200, 1, 1, 0);
  //w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(0.1, 0.3f, 0.1).translate(1.6f, 1.2, 0.6f), lamp_mat)); // Lamp leg
  //w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.3, 0.3f, 0.3).translate(1.6f, 1.7f, 0.6f), lamp_mat)); // Lamp bulb

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(2, 2, 2), rt::Point(1.6f, 1.7f, 0.6f)));
}

// Spheres falling with gravity and wind
void falling_spheres(rt::World& w) {
  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0, 0, 1.5);
//  w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id().translate(0, -10, 0), floor_mat));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(1000, 1000, 1000).translate(0, -1010, 0), floor_mat));
  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 10000000000000.0f, true));

  //for (int i = 0; i < 3; ++i) {
  //  float rand_x = std::rand()/((RAND_MAX + 1u)/16) - 8.0f;
  //  float rand_y = 10;// + std::rand()/((RAND_MAX + 1u)/5) - 2.5f;
  //  float rand_z = std::rand()/((RAND_MAX + 1u)/5) - 2.5f;
  //  float rand_scale = 1 + std::rand()/((RAND_MAX + 1u)/3);

  //  // TODO: make color table
  //  rt::Material sphere_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  //  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(rand_scale, rand_scale, rand_scale).translate(rand_x, rand_y, rand_z), sphere_mat));

  //  // TODO: check why weak_ptr is not moving correctly
  //  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), rand_scale));
  //}

  // Sphere 1
  rt::Material sphere1_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-6, 10, -2), sphere1_mat));
  // TODO: check why weak_ptr is not moving correctly
  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 1));

  rt::Material sphere3_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-10, 15, -2), sphere3_mat));
  // TODO: check why weak_ptr is not moving correctly
  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 1));

  // Sphere 2
  rt::Material sphere2_mat(rt::Color(0.2f, 0.8f, 0.2f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(6, 10, -2).scale(2, 2, 2), sphere2_mat));
  // TODO: check why weak_ptr is not moving correctly
  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 2));

  // Transparent liquid box
  rt::Material box_mat(rt::white, 0.1f, 0.9f, 0, 0, 0, 1, 1.5);
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(5, 5, 5).translate(-8, 3, 0), box_mat));
  w.objects().back()->behaviors().emplace_back(std::make_shared<Physics::Liquid>(w, w.objects().back().get(), 1));

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
}

void friction_box(rt::World& w) {
  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0, 0, 1.5);
  w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id().translate(0, -2, 0), floor_mat));

  rt::Material cube_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().scale(4, 2, 2).translate(-10, 0, 0), cube_mat));

  // TODO: check why weak_ptr is not moving correctly
  w.objects()[0]->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 2.0f, false, rt::Vector(1, 0, 0)));
}

void rotating_box(rt::World& w) {
  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));
//  rt::Material floor_mat(rt::Color(80.0f / 255, 5.0f / 255, 94.0f/255), 0.1f, 0.9f, 0, 0, 0, 0, 1.5);
//  w.objects().emplace_back(std::make_shared<rt::Plane>(rt::Transform::id().translate(0, -2, 0), floor_mat));

  rt::Material cube_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200, 1, 0, 1);
  w.objects().emplace_back(std::make_shared<rt::Cube>(rt::Transform::id().translate(0, 8, 0), cube_mat)); //.scale(2, 4, 2)

  // TODO: check why weak_ptr is not moving correctly
  w.objects()[0]->behaviors().emplace_back(std::make_shared<Physics::RigidBody>(w, w.objects().back().get(), 2.0f, false)); //, rt::Vector(0.2f, -0.2f, 0.2f), rt::zero_vec, rt::Vector(0.12f, 0, 0.25f), rt::zero_vec));
  std::map<int, rt::Vector> linearKeyMap;
  linearKeyMap[GLFW_KEY_W] = rt::Vector(0, 0.25, 0);
  linearKeyMap[GLFW_KEY_S] = rt::Vector(0, -0.25, 0);
  gInputSystems.emplace_back(std::make_shared<Physics::LinearInputSystem>(w.objects().back().get(), linearKeyMap));
  w.objects()[0]->behaviors().push_back(gInputSystems.back());
  std::map<int, rt::Vector> angularKeyMap;
  angularKeyMap[GLFW_KEY_A] = rt::Vector(0, 0, 0.01);
  angularKeyMap[GLFW_KEY_D] = rt::Vector(0, 0, -0.01);
  gInputSystems.emplace_back(std::make_shared<Physics::AngularInputSystem>(w.objects().back().get(), angularKeyMap));
  w.objects()[0]->behaviors().push_back(gInputSystems.back());
}

int main(int argc, char* argv[])
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(static_cast<int>(res.first), static_cast<int>(res.second), "Ray Tracer GL", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Set the right side up (flip the image) */
  glRasterPos2f(-1,1);
  glPixelZoom( 1, -1 );

  // Setup key input
  glfwSetKeyCallback(window, process_input);

  // Setup the world
  rt::World w;
  std::unique_ptr<Physics::Engine> physics = std::make_unique<Physics::Engine>(w, 1e-8);

  //falling_spheres(w);
  //friction_box(w);
  rotating_box(w);
  w.setup();

  rt::Profiler p(true);

  size_t frame_num = 0;
  short fps_num = 0;
  short fps = 0;
  
  auto start = std::chrono::high_resolution_clock::now(); 

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    
    {
      auto profile = p.profile("Render frame");
      rt::Camera cam(res, FOV, rt::Transform::id().view(FROM, TO, UP));
      canvas = std::make_unique<rt::Canvas>(std::move(cam.render(w)));
    }
    
    {
      auto profile = p.profile("Draw frame");
      glDrawPixels(static_cast<GLsizei>(res.first), static_cast<GLsizei>(res.second), GL_RGBA, GL_FLOAT, canvas->canvas());

      /* Swap front and back buffers */
      glfwSwapBuffers(window);
    }

    ++frame_num;
    ++fps_num;
    auto end_frame = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_frame - start); 
    if (duration >= 1s) {
      fps = fps_num;
      fps_num = 0;
      start = end_frame;
    }

    std::cerr << "Frame: " << frame_num << ", frame in second: " << fps_num << ", FPS: " << fps << "\n";

    if (gPlayMode) {
      auto profile = p.profile("Physics");
      physics->act();
      w.act();
    }

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}