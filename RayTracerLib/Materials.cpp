#include "Materials.h"

namespace RayTracer {

  bool operator==(const Material& lhs, const Material& rhs) {
    return lhs.color == rhs.color &&
      ApproxEqual(lhs.ambient, rhs.ambient, epsilon) &&
      ApproxEqual(lhs.diffuse, rhs.diffuse, epsilon) &&
      ApproxEqual(lhs.specular, rhs.specular, epsilon) &&
      ApproxEqual(lhs.shininess, rhs.shininess, epsilon);
  }

  bool operator!=(const Material& lhs, const Material& rhs) {
    return !(lhs == rhs);
  }

  Color lighting(Material m, PointLight light, Point p, Vector eyev, Vector normalv)
  {
    // Combine the surface color with the light's color
    auto effective_color = m.color * light.intensity;

    // Find the direction to the light source
    auto lightv = normalize(light.position - p);

    // Compute the ambient contribution
    auto ambient = effective_color * m.ambient;

    Color diffuse = black;
    Color specular = black;

    // Calculate the cosine of the angle between the light and normal vector
    // Negative cosine means the light is on the other side of the surface
    auto light_normal_angle = dot(lightv, normalv);
    if (light_normal_angle >= 0) {
      // Compute the diffuse contribution
      diffuse = effective_color * m.diffuse * light_normal_angle;

      // Calculate the cosine of the angle between the reflection and the eye
      // Negative value means light reflects away from the eye
      auto reflectv = reflect(-lightv, normalv);
      auto reflect_eye_angle = dot(reflectv, eyev);
      if (reflect_eye_angle > 0) {
        // Compute the specular contribution
        auto factor = powf(reflect_eye_angle, m.shininess);
        specular = light.intensity * m.specular * factor;
      }
    }

    return ambient + diffuse + specular;
  }

} // namespace RayTracer
