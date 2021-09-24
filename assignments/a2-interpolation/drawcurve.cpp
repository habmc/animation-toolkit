#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class DrawCubic : public atkui::Framework
{
public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400)
  {
  }

  virtual void scene()
  {
    setColor(vec3(0, 0, 1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    setColor(vec3(1, 1, 0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);

    // draw curve
    setColor(vec3(0, 0, 1));
    for (int i = 0; i < sizeof(points) / sizeof(points[0]); i++)
    {
      drawSphere(points[i], 3);
    }
  }

  void keyUp(int key, int mod)
  {
    if (key == GLFW_KEY_1)
    {
      // draw Bernstein
      int count = 0;
      for (float t = 0.0f; t < 1.0f; t += 0.02f)
      {
        vec3 pt = static_cast<float>(pow(1 - t, 3.0f)) * B0 + 3 * t * static_cast<float>(pow(1 - t, 2.0f)) * B1 + 3 * static_cast<float>(pow(t, 2.0f)) * (1 - t) * B2 + static_cast<float>(pow(t, 3.0f)) * B3;
        points[count++] = pt;
      }
    }
    else if (key == GLFW_KEY_2)
    {
      // draw Casteljau
      int count = 0;
      for (float t = 0.0f; t < 1.0f; t += 0.01f)
      {
        vec3 b01 = B0 * (1 - t) + B1 * t;
        vec3 b11 = B1 * (1 - t) + B2 * t;
        vec3 b21 = B2 * (1 - t) + B3 * t;
        vec3 b02 = b01 * (1 - t) + b11 * t;
        vec3 b12 = b11 * (1 - t) + b21 * t;
        vec3 b30 = b02 * (1 - t) + b12 * t;
        points[count++] = b30;
      }
    }
  }

private:
  vec3 B0 = vec3(100, 50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  vec3 points[100];
};

int main(int argc, char **argv)
{
  DrawCubic viewer;
  viewer.run();
}
