#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   Motion spliceUpperBody(const Motion &lower, const Motion &upper, float alpha)
   {
      Motion result;
      result.setFramerate(lower.getFramerate());

      // start bfs from spine 1 to get the ids of all keys in the upper body
      Joint *currentJoint = _skeleton.getByName("Beta:Spine1");
      std::deque<int> queue;
      queue.push_back(currentJoint->getID());
      std::vector<int> ids;

      while (!queue.empty())
      {
         currentJoint = _skeleton.getByID(queue.front());
         ids.push_back(currentJoint->getID());
         queue.pop_front();
         for (int i = 0; i < currentJoint->getNumChildren(); i++)
         {
            queue.push_back(currentJoint->getChildAt(i)->getID());
         }
      }

      for (int i = 0; i < lower.getNumKeys(); i++)
      {
         Pose lowerPose = lower.getKey(i);
         Pose upperPose = upper.getKey(i + 120);

         for (int j = 0; j < ids.size(); j++)
         {
            lowerPose.jointRots[ids[j]] = glm::slerp(upperPose.jointRots[ids[j]], lowerPose.jointRots[ids[j]], alpha);
         }
         result.appendKey(lowerPose);
      }
      return result;
   }

   void scene()
   {
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: " + std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha + 0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha - 0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char **argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}
