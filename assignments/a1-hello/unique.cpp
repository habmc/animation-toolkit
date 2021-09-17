#include "atkui/framework.h"
#include <math.h>

using namespace glm;

class Unique : public atkui::Framework
{
public:
  Unique() : atkui::Framework(atkui::Orthographic)
  {
  }

  virtual void setup()
  {
    baseColor = vec3(0.7, 0.4, 0.6);
    numPetals = 3;
    size = 200;
  }

  virtual void scene()
  {
    float r1 = -0.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
    float r2 = -0.2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
    float r3 = -0.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
    vec3 color = baseColor + vec3(r1, r2, r3);

    /* if the amount of time since the previous frame >= 1/60s, update numPetals and size */
    if (dt() * 60 >= 1)
    {
      if (numPetals > 10)
      {
        numPetals = 3;
        size = 200;
      }
      for (int i = 0; i < 360; i++)
      {
        // the equation of a rose curve
        float r = size * sin(M_PI * numPetals * i / 180);

        // converting to cartesian co-ordinates
        float x = r * cos(M_PI * i / 180);
        float y = r * sin(M_PI * i / 180);
        points[i] = vec3(x + width() / 2, y + height() / 2, 0);
      }
      numPetals += 1;
      size -= 5;
    }

    for (int i = 0; i < 360; i++)
    {
      setColor(color);
      drawSphere(points[i], 5);
    }
  }

private:
  int numPetals;
  int size;
  vec3 points[360];
  vec3 baseColor;
};

int main(int argc, char **argv)
{
  Unique viewer;
  viewer.run();
  return 0;
}