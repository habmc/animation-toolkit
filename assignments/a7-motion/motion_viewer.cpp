#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective)
   {
   }
   MotionViewer(std::string filePath) : atkui::Framework(atkui::Perspective)
   {
      bvhPath = filePath;
   }

   void setup()
   {
      BVHReader reader;
      reader.load(bvhPath, skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene()
   {
      if (!paused)
      {
         time += dt() * timeScale;
         currentFrame = floor(time * motion.getFramerate());
      }
      if (currentFrame > motion.getNumKeys())
      {
         currentFrame = 0;
         time = 0;
      }
      else if (currentFrame < 0)
      {
         currentFrame = motion.getNumKeys() - 1;
         time = currentFrame / motion.getFramerate();
      }
      motion.update(skeleton, time);

      setColor(vec3(0, 0, 0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++)
      {
         Joint *joint = skeleton.getByID(i);
         if (joint->getParent() == 0)
            continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      drawText(paused ? "Paused" : "Playing", 10, 15);
      drawText("Current frame: " + std::to_string(currentFrame), 10, 35);
      drawText("Time scale: " + std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == 'P')
      {
         paused = paused ? false : true;
      }
      else if (key == 'O')
      {
         time = 0;
         currentFrame = 0;
      }
      else if (paused && key == '.')
      {
         currentFrame++;
         time = currentFrame / motion.getFramerate();
      }
      else if (paused && key == ',')
      {
         currentFrame--;
         time = currentFrame / motion.getFramerate();
      }
      else if (key == '[')
      {
         timeScale /= 2;
      }
      else if (key == ']')
      {
         timeScale *= 2;
      }
   }

private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0;
   bool paused = false;
   float time = 0;
   std::string bvhPath = "../motions/Beta/jump.bvh";
};

int main(int argc, char **argv)
{
   if (argc > 1)
   {
      MotionViewer viewer(argv[1]);
      viewer.run();
   }
   else
   {
      MotionViewer viewer;
      viewer.run();
   }
}
