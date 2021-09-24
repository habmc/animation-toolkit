#include "atkui/framework.h"
using namespace glm;

class Screensaver : public atkui::Framework
{
  struct Curve
  {
    vec3 points[200];
    vec3 color;
  };

public:
  Screensaver() : atkui::Framework(atkui::Orthographic)
  {
  }

  void setup()
  {
    for (int i = 0; i < 2; i += 1)
    {
      vec3 B0 = getRandomPointWithinRange();
      vec3 B1 = getRandomPointWithinRange();
      vec3 B2 = getRandomPointWithinRange();
      vec3 B3 = getRandomPointWithinRange();

      int count = 0; // number of points initialized
      Curve newCurve;
      for (float t = 0.0f; t < 1.0f; t += 0.005f)
      {
        vec3 pt = static_cast<float>(pow(1 - t, 3.0f)) * B0 + 3 * t * static_cast<float>(pow(1 - t, 2.0f)) * B1 + 3 * static_cast<float>(pow(t, 2.0f)) * (1 - t) * B2 + static_cast<float>(pow(t, 3.0f)) * B3;
        newCurve.points[count++] = pt;
      }
      newCurve.color = vec3(getRandomColorCode(), getRandomColorCode(), getRandomColorCode());
      curves.push_back(newCurve);
      if (i == 0)
      {
        startCurve = newCurve;
      }
      else
      {
        destCurve = newCurve;
      }
    }
  }

  void scene()
  {
    while (curves.size() > maxNumCurves)
    {
      curves.pop_front();
    }

    std::list<Curve>::iterator it;

    // draw all exisiting curves
    for (it = curves.begin(); it != curves.end(); ++it)
    {
      Curve curr = *it;
      setColor(curr.color);
      for (int j = 0; j < 200; j++)
      {
        drawSphere(curr.points[j], 2);
      }
    }

    Curve newCurve;
    // draw the new interpolated curve
    for (int i = 0; i < 200; i++)
    {
      vec3 B0 = startCurve.points[i];
      vec3 B3 = destCurve.points[i];
      vec3 pt = static_cast<float>(pow(1 - t, 3.0f)) * B0 + 3 * t * static_cast<float>(pow(1 - t, 2.0f)) * P1 + 3 * static_cast<float>(pow(t, 2.0f)) * (1 - t) * P2 + static_cast<float>(pow(t, 3.0f)) * B3;
      drawSphere(pt, 2);
      newCurve.points[i] = pt;
    }

    elapsed += dt();
    // add newCurve to the list if more 0.1 seconds have passed since last curve was added
    if (elapsed > 0.1)
    {
      elapsed = 0.0f;
      newCurve.color = destCurve.color;
      curves.push_back(newCurve);
    }

    t += 0.025f;
    if (t >= 1)
    {
      /* generate a new destination curve after 40 frames */
      t = 0.0f;
      std::copy(std::begin(destCurve.points), std::end(destCurve.points), std::begin(startCurve.points));
      vec3 B0 = getRandomPointWithinRange();
      vec3 B1 = getRandomPointWithinRange();
      vec3 B2 = getRandomPointWithinRange();
      vec3 B3 = getRandomPointWithinRange();

      int count = 0;
      for (float i = 0.0f; i < 1.0f; i += 0.005f)
      {
        vec3 pt = static_cast<float>(pow(1 - i, 3.0f)) * B0 + 3 * i * static_cast<float>(pow(1 - i, 2.0f)) * B1 + 3 * static_cast<float>(pow(i, 2.0f)) * (1 - i) * B2 + static_cast<float>(pow(i, 3.0f)) * B3;
        destCurve.points[count++] = pt;
      }
      destCurve.color = vec3(getRandomColorCode(), getRandomColorCode(), getRandomColorCode());
      curves.push_back(destCurve);
    }
  }

private:
  std::list<Curve> curves;
  Curve startCurve;
  Curve destCurve;
  int maxNumCurves = 50;
  float elapsed = 0.0f;                  // variable to check how much time has passed since the last insertion to curves
  float t = 0.0f;                        // variable to store how much time has passed since the last destination curve was generated
  vec3 P1 = getRandomPointWithinRange(); // new control point for interpolation
  vec3 P2 = getRandomPointWithinRange(); // new control point for interpolation

  float getRandomColorCode()
  {
    float code = 0.5 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.4));
    return code;
  }

  vec3 getRandomPointWithinRange()
  {
    return vec3(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / width())), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / height())), 0);
  }
};

int main(int argc, char **argv)
{
  Screensaver viewer;
  viewer.run();
}
