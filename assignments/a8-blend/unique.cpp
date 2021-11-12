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
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   class Particle
   {
   public:
      vec3 position;
      float creationTime;
      float size = 7.0f;

      Particle(vec3 pos, float time)
      {
         position = pos;
         creationTime = time;
      }

      void updateSize(float time)
      {
         float maxLen = 2.0f;
         float elapsed = time - creationTime;
         if (elapsed > maxLen)
         {
            size = 0.0f;
         }
         else
         {
            size = (1 - elapsed / maxLen) * 5.0f;
         }
      }
   };

   Motion spliceUpperBody(const Motion &lower, const Motion &upper, float alpha)
   {
      Motion result;
      result.setFramerate(lower.getFramerate());
      Joint *currentJoint = _skeleton.getByName("Beta:Spine1");
      std::deque<int> queue;
      queue.push_back(currentJoint->getID());
      std::vector<int> ids;
      while (!queue.empty())
      {
         currentJoint = _skeleton.getByID(queue.front());
         queue.pop_front();
         ids.push_back(currentJoint->getID());
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
      t += dt();
      _splice.update(_skeleton, t * 0.5);

      setColor(vec3(0.82, 0.82, 0.82));
      vec3 leftToePos = _skeleton.getByName("Beta:LeftToeBase")->getGlobalTranslation();
      vec3 rightToePos = _skeleton.getByName("Beta:RightToeBase")->getGlobalTranslation();

      for (Particle &particle : particles)
      {
         particle.updateSize(t);
      }

      while (!particles.empty() && particles[0].size == 0)
      {
         particles.pop_front();
      }

      particles.push_back(Particle(leftToePos, t));
      particles.push_back(Particle(rightToePos, t));

      for (Particle &particle : particles)
      {
         drawSphere(particle.position, particle.size);
      }

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
      else if (key == GLFW_KEY_J)
      {
         reader.load("../motions/Beta/jump.bvh", _skeleton, _lower);
         t = 0.0f;
         particles.clear();
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_W)
      {
         reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
         t = 0.0f;
         particles.clear();
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_R)
      {
         reader.load("../motions/Beta/standard_run.bvh", _skeleton, _lower);
         t = 0.0f;
         particles.clear();
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
   float t = 0.0f;
   BVHReader reader;
   std::deque<Particle> particles = {};
};

int main(int argc, char **argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}