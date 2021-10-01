#include "atkui/framework.h"
#include "spline.h"
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }
   virtual void setup()
   {
      keys.push_back(glm::vec3(180, 280, 0));
      keys.push_back(glm::vec3(180, 320, 0));
      keys.push_back(glm::vec3(220, 360, 0));
      keys.push_back(glm::vec3(260, 360, 0));
      keys.push_back(glm::vec3(260, 200, 0));
      keys.push_back(glm::vec3(300, 200, 0));
      keys.push_back(glm::vec3(300, 160, 0));
      keys.push_back(glm::vec3(180, 160, 0));
      keys.push_back(glm::vec3(180, 200, 0));
      keys.push_back(glm::vec3(220, 200, 0));
      keys.push_back(glm::vec3(220, 320, 0));
      keys.push_back(glm::vec3(180, 280, 0));
      computeControlPoints();
   }

   virtual void scene()
   {
      setColor(vec3(1, 0.5, 1));
      float t = elapsedTime();
      int numSegments = keys.size() - 1;
      float segmentDuration = 1.0f;
      if (t <= numSegments)
      {
         int segment = (int)(t / segmentDuration);
         float u = (fmod(t, segmentDuration));
         glm::vec3 B0 = mCtrlPoints[segment * 3];
         glm::vec3 B1 = mCtrlPoints[segment * 3 + 1];
         glm::vec3 B2 = mCtrlPoints[segment * 3 + 2];
         glm::vec3 B3 = mCtrlPoints[segment * 3 + 3];
         glm::vec3 point = glm::vec3((float)(pow(1 - u, 3)) * B0 + (float)(3 * u * pow(1 - u, 2)) * B1 + (float)(3 * pow(u, 2) * (1 - u)) * B2 + (float)(pow(u, 3)) * B3);
         allPoints.push_back(point);
      }
      else
      {
         if (negateXChange)
         {
            xChange *= -1;
            negateXChange = false;
         }
         for (int i = 0; i < allPoints.size(); i++)
         {
            allPoints[i][0] += xChange;
            if (allPoints[i][0] >= width() || allPoints[i][0] <= 0)
            {
               negateXChange = true;
            }
         }
      }

      for (int i = 0; i < allPoints.size(); i++)
      {
         drawSphere(allPoints[i], 5);
      }
   }

   virtual void computeControlPoints()
   {
      for (int i = 0; i < keys.size(); i++)
      {
         mCtrlPoints.push_back(keys[i]);

         if (i + 1 < keys.size())
         {
            if (i > 0)
            {
               glm::vec3 subtracted = keys[i + 1] - keys[i - 1];
               glm::vec3 b1 = keys[i] + glm::vec3(subtracted[0] / 6, subtracted[1] / 6, subtracted[2] / 6);
               mCtrlPoints.push_back(b1);
            }
            else
            {
               glm::vec3 subtracted = keys[i + 1] - keys[i];
               glm::vec3 b1 = keys[i] + glm::vec3(subtracted[0] / 6, subtracted[1] / 6, subtracted[2] / 6);
               mCtrlPoints.push_back(b1);
            }

            if (i + 2 < keys.size())
            {
               glm::vec3 subtracted = keys[i + 2] - keys[i];
               glm::vec3 b2 = keys[i + 1] - glm::vec3(subtracted[0] / 6, subtracted[1] / 6, subtracted[2] / 6);
               mCtrlPoints.push_back(b2);
            }
            else
            {
               glm::vec3 subtracted = keys[keys.size() - 1] - keys[i];
               glm::vec3 b2 = keys[i + 1] - glm::vec3(subtracted[0] / 6, subtracted[1] / 6, subtracted[2] / 6);
               mCtrlPoints.push_back(b2);
            }
         }
      }
   }

private:
   std::vector<glm::vec3> mCtrlPoints;
   std::vector<glm::vec3> allPoints;
   std::vector<glm::vec3> keys;
   int xChange = 5;
   bool negateXChange = false;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}
