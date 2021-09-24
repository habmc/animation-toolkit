#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework
{
public:
  Look() : atkui::Framework(atkui::Orthographic)
  {
  }

  virtual void setup()
  {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
    movingTarget = false;
    leftPupilPosition = vec3(width() / 3 + 15, 0.5 * height(), 0);
    rightPupilPosition = vec3(2 * width() / 3 - 15, 0.5 * height(), 0);
  }

  virtual void scene()
  {
    setColor(vec3(1, 1, 1));
    drawSphere(vec3(width() / 3, height() / 2, -50), eyeRadius);
    drawSphere(vec3(2 * width() / 3, height() / 2, -50), eyeRadius);

    setColor(vec3(0, 0, 0));
    drawSphere(leftPupilPosition, pupilRadius);
    drawSphere(rightPupilPosition, pupilRadius);

    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1, 0, 0));
    drawSphere(target, targetRadius);
  }

  void mouseMotion(int pX, int pY, int dx, int dy)
  {
    if (movingTarget)
    {
      _mouseX = pX;
      _mouseY = height() - pY;

      vec3 leftPivotPosition = vec3(width() / 3, 0.5 * height(), 0);
      vec3 rightPivotPosition = vec3(2 * width() / 3, 0.5 * height(), 0);

      float leftTheta = atan2(_mouseY - leftPupilPosition[1], _mouseX - leftPupilPosition[0]);
      leftPupilPosition = vec3(15 * cos(leftTheta) + leftPivotPosition[0], 15 * sin(leftTheta) + leftPivotPosition[1], 0);

      float rightTheta = atan2(_mouseY - rightPupilPosition[1], _mouseX - rightPupilPosition[0]);
      rightPupilPosition = vec3(15 * cos(rightTheta) + rightPivotPosition[0], 15 * sin(rightTheta) + rightPivotPosition[1], 0);
    }
  }

  void mouseDown(int button, int mods)
  {
    vec2 currentPosition = this->mousePosition();
    if (button == GLFW_MOUSE_BUTTON_LEFT && length(currentPosition - vec2(_mouseX, height() - _mouseY)) <= targetRadius)
    {
      movingTarget = true;
    }
  }

  void mouseUp(int button, int mods)
  {
    movingTarget = false;
  }

private:
  int _mouseX;
  int _mouseY;
  bool movingTarget; // whether the target is being moved
  vec3 leftPupilPosition;
  vec3 rightPupilPosition;
  const double pupilRadius = 30;
  const double eyeRadius = 80;
  const double targetRadius = 10;
};

int main(int argc, char **argv)
{
  Look viewer;
  viewer.run();
  return 0;
}
