#include <GLFW/glfw3.h>

#include <chrono>
using namespace std::chrono_literals;

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Camera.h"
#include "../RayTracerLib/Profiler.h"

namespace rt = RayTracer;

int main(void)
{
  auto res = rt::Camera::RES_720P;

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

  rt::Point FROM(0, 1.5f, -5);
  rt::Point TO(0, 1, 0);
  rt::Vector UP(0, 1, 0);

  rt::Camera cam(res, 3.14159f / 3.0f, rt::Transform::id().view(FROM, TO, UP));
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
    
    std::unique_ptr<rt::Canvas> canvas;
    {
      auto profile = p.profile("Render frame");
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