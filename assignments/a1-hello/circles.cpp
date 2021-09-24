<<<<<<< HEAD
#include "atkui/framework.h"
#include <math.h>

using namespace glm;
class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic)
   {
   }

   virtual void setup()
   {
      center = vec3(width() / 2, height() / 2, 0);
      int radius = 0;
      for (int i = 0; i < numLayers; i++)
      {
         colors[i] = pallet[i % 5];
         radiusArray[i] = radius;
         for (int j = 0; j < numCircles; j++)
         {
            long double theta = M_PI * 10 * j / 180;
            long double p_x = center[0] + cos(theta) * radius;
            long double p_y = center[1] + sin(theta) * radius;
            circles[i][j] = vec3(p_x, p_y, 0);
         }
         radius += 15;
      }
   }

   virtual void scene()
   {
      for (int i = 0; i < numLayers; i++)
      {
         int radius = radiusArray[i];
         int factor = -1;
         for (int j = 0; j < numCircles; j++)
         {
            long double theta = atan2(circles[i][j][1] - center[1], circles[i][j][0] - center[0]);
            theta += pow(factor, i) * dt() * 10;
            long double p_x = center[0] + cos(theta) * radius;
            long double p_y = center[1] + sin(theta) * radius;
            circles[i][j] = vec3(p_x, p_y, 0);
            setColor(colors[i]);
            drawSphere(circles[i][j], 10);
         }
      }
   }

private:
   static const int numLayers = 15;
   static const int numCircles = 36;
   vec3 center;
   vec3 circles[numLayers][numCircles];
   int radiusArray[numLayers];
   vec3 colors[numLayers];
   std::vector<vec3> pallet =
       {
           vec3(0, 165, 227) / 255.0f,
           vec3(141, 215, 191) / 255.0f,
           vec3(255, 150, 197) / 255.0f,
           vec3(255, 87, 104) / 255.0f,
           vec3(255, 162, 58) / 255.0f};
};

int main(int argc, char **argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
=======
#include "atkui/framework.h"

using namespace glm;
class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
   }

   virtual void scene() {
   }

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
>>>>>>> upstream/main
