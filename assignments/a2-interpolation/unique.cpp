/*
 * Ha Phan
 * Implement a degree-4 Bezier curve based on the control points that users specify with mouse drag
 */
#include "atkui/framework.h"
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }

   virtual void scene()
   {
      /* draw all control points and connect them by lines */
      setColor(vec3(0, 0, 1));
      drawSphere(B0, pointRadius);
      drawSphere(B4, pointRadius);
      setColor(vec3(1, 0, 0));
      drawSphere(B1, pointRadius);
      drawSphere(B2, pointRadius);
      drawSphere(B3, pointRadius);
      int count = 0;
      setColor(vec3(0, 1, 1));
      drawLine(B0, B1);
      drawLine(B1, B2);
      drawLine(B2, B3);
      drawLine(B3, B4);

      /* draw a degree-4 Bezier curve */
      setColor(vec3(0.4, 0.7, 0.5));
      for (float t = 0.0f; t < 1.0f; t += 0.01f)
      {
         vec3 pt = static_cast<float>(pow(1 - t, 4.0f)) * B0 + 4 * t * static_cast<float>(pow(1 - t, 3.0f)) * B1 + 6 * static_cast<float>(pow(t, 2.0f)) * static_cast<float>(pow(1 - t, 2.0f)) * B2 + 4 * static_cast<float>(pow(t, 3.0f)) * (1 - t) * B3 + static_cast<float>(pow(t, 4.0f)) * B4;
         drawSphere(pt, 3);
      }
   }

   void mouseDown(int button, int mods)
   {
      vec2 currentPosition = this->mousePosition();
      if (button == GLFW_MOUSE_BUTTON_LEFT && length(currentPosition - vec2(B1[0], height() - B1[1])) <= pointRadius)
      {
         movingB1 = true;
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT && length(currentPosition - vec2(B2[0], height() - B2[1])) <= pointRadius)
      {
         movingB2 = true;
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT && length(currentPosition - vec2(B3[0], height() - B3[1])) <= pointRadius)
      {
         movingB3 = true;
      }
   }
   void mouseMotion(int pX, int pY, int dx, int dy)
   {
      if (movingB1)
      {
         B1 = vec3(pX, height() - pY, 0);
      }
      else if (movingB2)
      {
         B2 = vec3(pX, height() - pY, 0);
      }
      else if (movingB3)
      {
         B3 = vec3(pX, height() - pY, 0);
      }
   }

   void mouseUp(int button, int mods)
   {
      if (movingB1)
      {
         movingB1 = false;
      }
      else if (movingB2)
      {
         movingB2 = false;
      }
      else if (movingB3)
      {
         movingB3 = false;
      }
   }

private:
   vec3 B0 = vec3(100, 200, 0);
   vec3 B1 = vec3(150, 100, 0);
   vec3 B2 = vec3(250, 180, 0);
   vec3 B3 = vec3(300, 300, 0);
   vec3 B4 = vec3(400, 250, 0);
   bool movingB1, movingB2, movingB3 = false;
   int pointRadius = 10;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}