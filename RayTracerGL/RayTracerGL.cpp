#include <GLFW/glfw3.h>

#include "../RayTracerLib/World.h"
#include "../RayTracerLib/Sphere.h"
#include "../RayTracerLib/Camera.h"

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

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    
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

    auto canvas = cam.render(w);

    glRasterPos2f(-1,1);
    glPixelZoom( 1, -1 );
    glDrawPixels(res.first, res.second, GL_RGBA, GL_FLOAT, canvas.canvas());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}