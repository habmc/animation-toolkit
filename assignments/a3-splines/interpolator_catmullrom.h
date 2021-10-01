#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"
#include <iostream>

//--------------------------------
// Catmull-Rom
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
   InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

   virtual glm::vec3 interpolate(int segment, double u) const
   {
      glm::vec3 B0 = mCtrlPoints[segment * 3];
      glm::vec3 B1 = mCtrlPoints[segment * 3 + 1];
      glm::vec3 B2 = mCtrlPoints[segment * 3 + 2];
      glm::vec3 B3 = mCtrlPoints[segment * 3 + 3];
      return glm::vec3((float)(pow(1 - u, 3)) * B0 + (float)(3 * u * pow(1 - u, 2)) * B1 + (float)(3 * pow(u, 2) * (1 - u)) * B2 + (float)(pow(u, 3)) * B3);
   }

   virtual void
   computeControlPoints(const std::vector<glm::vec3> &keys)
   {
      clearControlPoints();
      if (keys.size() < 2)
      {
         return;
      }
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
};

#endif
