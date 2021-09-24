#include "atkui/framework.h"
using namespace glm;

class ParticleCubic : public atkui::Framework
{
public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic)
  {
  }

  void setup()
  {
    int count = 0;
    for (float t = 0.0f; t < 1.0f; t += 0.01f)
    {
      vec3 pt = static_cast<float>(pow(1 - t, 3.0f)) * B0 + 3 * t * static_cast<float>(pow(1 - t, 2.0f)) * B1 + 3 * static_cast<float>(pow(t, 2.0f)) * (1 - t) * B2 + static_cast<float>(pow(t, 3.0f)) * B3;
      linePoints[count++] = pt;
    }
  }

  void scene()
  {      
    setColor(vec3(0.3, 0.6, 0.5));
    for (int i = 0; i < sizeof(linePoints) / sizeof(linePoints[0]); i++)
    {
      drawSphere(linePoints[i], 2);
    }
    float t = fmod(elapsedTime(), 5.0f);
    t = t / 5;
    vec3 pt = static_cast<float>(pow(1 - t, 3.0f)) * B0 + 3 * t * static_cast<float>(pow(1 - t, 2.0f)) * B1 + 3 * static_cast<float>(pow(t, 2.0f)) * (1 - t) * B2 + static_cast<float>(pow(t, 3.0f)) * B3;
    setColor(vec3(1, 0, 0));
    drawSphere(vec3(pt), 10);
  }

private:
  vec3 B0 = vec3(100, 50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  vec3 linePoints[100];
};

int main(int argc, char **argv)
{
  ParticleCubic viewer;
  viewer.run();
}
