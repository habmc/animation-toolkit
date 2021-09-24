#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework
{
public:
  Gradient() : atkui::Framework(atkui::Orthographic)
  {
    for (int i = 0; i < width(); i++)
    {
      for (int j = 0; j < height(); j++)
      {
        vec3 c0 = vec3(1, 1, 0) * (1 - i / width()) + vec3(0, 1, 1) * (i / width());
        vec3 c1 = vec3(1, 0, 0) * (1 - i / width()) + vec3(1, 0, 1) * (i / width());
        vec3 c = c0 * (1 - j / height()) + c1 * (j / height());
        colors[i][j] = c;
      }
    }
  }

  virtual void scene()
  {
    for (int i = 0; i < width(); i++)
    {
      for (int j = 0; j < height(); j++)
      {
        setColor(colors[i][j]);
        // y-coordinate = height() - j - 1 since we start from bottom left corner
        drawCube(vec3(i, height() - j - 1, 0), vec3(1, 1, 0));
      }
    }
  }

private:
  vec3 colors[500][500];
};

int main(int argc, char **argv)
{
  Gradient viewer;
  viewer.run();
}
