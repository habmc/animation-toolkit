#include <iostream>
#include "atkui/framework.h"
#include "atkmath/matrix3.h"
#include <Eigen/Dense>
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }
   virtual void setup()
   {
      _mouseX = width() * 0.5;
      _mouseY = height() * 0.5;
      movingTarget = false;

      for (int i = 5; i < width(); i += 10)
      {
         for (int j = 0; j < height(); j += 15)
         {
            cuboidPositions.push_back(vec3(i, j, 0));
         }
      }
   }

   virtual void scene()
   {
      for (int i = 0; i < cuboidPositions.size(); i += 1)
      {
         setColor(vec3(0.5, 0.8, 0.5));
         vec3 cuboid = cuboidPositions[i];
         if (movingTarget && pow(cuboidPositions[i][0] - _mouseX, 2) + pow(cuboidPositions[i][1] - _mouseY, 2) <= pow(100, 2))
         {
            float theta = atan2(_mouseY - cuboid[1], _mouseX - cuboid[0]);
            push();
            translate(cuboid);
            rotate(theta, vec3(0, 0, 1));
            drawCube(vec3(0), vec3(10, 1, 0));
            pop();
         }
         else
         {
            drawCube(cuboid, vec3(10, 1, 0));
         }
      }

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
   std::vector<glm::vec3> cuboidPositions;
   const double targetRadius = 10;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
}
