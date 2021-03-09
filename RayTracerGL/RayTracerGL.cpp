#include <GLFW/glfw3.h>

#include <chrono>
using namespace std::chrono_literals;
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Camera.h"
#include "../RayTracerLib/Profiler.h"

namespace rt = RayTracer;

// TODO: find a better place for these (and design the GL app better in general)
rt::Point FROM(0, 1.5f, -5);
rt::Point TO(0, 1, 0);
rt::Vector UP(0, 1, 0);
auto res = rt::Camera::RES_720P;
float FOV = 3.14159f / 3.0f;
float MOVE_DELTA = 0.1f;
std::unique_ptr<rt::Canvas> canvas;

void process_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {
    switch (key) {
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
      glfwTerminate();
      exit(0);
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
}

int main(int argc, char* argv[])
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(res.first, res.second, "Ray Tracer", NULL, NULL);
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

  auto plane_scale = rt::Transform::id().scale(10, 0.1f, 10);
  rt::Material floor_mat(rt::Color(1, 0.9f, 0.9f), 0.1f, 0.9f, 0, 0);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(plane_scale, floor_mat));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(-0.7853975f /* -pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(0.7853975f /* pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  rt::Material middle_mat(rt::Color(0.1f, 1, 0.5f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-0.5f, 1, 0.5f), middle_mat));
  rt::Material right_mat(rt::Color(0.5f, 1, 0.1f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(1.5f, 0.5f, -0.5f), right_mat));
  rt::Material left_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(-1.5f, 0.33f, -0.75f), left_mat));

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));

  rt::Profiler p(true);

  size_t frame_num = 0;
  short fps_num = 0;
  short fps = 0;
  
  auto start = std::chrono::high_resolution_clock::now(); 

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    
    {
      auto profile = p.profile("Render frame");
      // TODO: add "move" API to camera
      rt::Camera cam(res, FOV, rt::Transform::id().view(FROM, TO, UP));
      canvas = std::make_unique<rt::Canvas>(std::move(cam.render(w)));
    }
    
    {
      auto profile = p.profile("Draw frame");
      glDrawPixels(res.first, res.second, GL_RGBA, GL_FLOAT, canvas->canvas());

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

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}