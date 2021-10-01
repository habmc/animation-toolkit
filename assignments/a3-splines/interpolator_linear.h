#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include "interpolator.h"
#include <iostream>

//--------------------------------
// Linear
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
   InterpolatorLinear() : Interpolator("Linear") {}
   virtual glm::vec3 interpolate(int segment, double u) const
   {
      return glm::vec3(mCtrlPoints[segment] * (1 - static_cast<float>(u)) + mCtrlPoints[segment + 1] * static_cast<float>(u));
   }

   virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
   {
      clearControlPoints();
      if (keys.size() < 2)
      {
         return;
      }
      for (glm::vec3 i : keys)
      {
         mCtrlPoints.push_back(i);
      }
   }
};

#endif
