#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective)
   {
   }

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/surprised.bvh", skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene()
   {
      if (!backward)
      {
         timeScale += 0.001f;
         time += dt() * timeScale;
      }
      else if (backward)
      {
         timeScale += 0.001f;
         time -= dt() * timeScale;
      }
      if (time > motion.getDuration())
      {
         time -= dt() * timeScale;
         timeScale = 1.0f;
         backward = true;
      }
      else if (time < 0)
      {
         time += dt() * timeScale;
         timeScale = 1.0f;
         backward = false;
      }
      motion.update(skeleton, time);

      setColor(vec3(0, 0.8, 0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++)
      {
         Joint *joint = skeleton.getByID(i);
         if (joint->getParent() == 0)
            continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 10);
      }
   }

private:
   Skeleton skeleton;
   Motion motion;
   float time = 0;
   float timeScale = 1.0f;
   bool backward = false;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
}
