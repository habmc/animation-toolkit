#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
public:
  Eyes() : atkui::Framework(atkui::Orthographic)
  {
  }

  virtual void scene()
  {
    float p_x = 20 * sin(elapsedTime() * 2) + width() / 3;
    float p_y = 0.5 * height();
    setColor(vec3(0, 0, 0));
    drawSphere(vec3(p_x, p_y, 0), 20);

    p_x = 20 * sin(elapsedTime() * 2) + 2 * width() / 3;
    p_y = 0.5 * height();
    drawSphere(vec3(p_x, p_y, 0), 20);

    setColor(vec3(1, 1, 1));
    double radius = 70;
    drawSphere(vec3(width() / 3, height() / 2, -50), radius);
    drawSphere(vec3(2 * width() / 3, height() / 2, -50), radius);
  }
};

int main(int argc, char **argv)
{
  Eyes viewer;
  viewer.run();
  return 0;
}
