#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      reader.load("../motions/Beta/idle.bvh", _skeleton_2, _motion_2);

      for (int i = 0; i < _motion_2.getNumKeys(); i++)
      {
         Pose newPose = _motion_2.getKey(i);
         newPose.rootPos = vec3(_motion.getKey(i).rootPos[0], _motion.getKey(i).rootPos[1], _motion.getKey(i).rootPos[2] + 200);
         newPose.jointRots[0] = glm::angleAxis((float)M_PI, vec3(0, 1, 0));
         _motion_2.editKey(i, newPose);
      }

      _motion.update(_skeleton, 0);
      _motion_2.update(_skeleton_2, 0);

      _spineOffset = _skeleton.getByName("Beta:Spine2")->getGlobalTranslation() + vec3(0, 15, 50);
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());
      _motion_2.update(_skeleton_2, elapsedTime());

      IKController ik;
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:Spine2")->getID(), _spineTarget, 0.001f);
      _skeleton.fk();
   }

   void scene()
   {
      update();
      _drawer.color = vec3(1, 0, 0);
      _drawer.draw(_skeleton, *this);
      _drawer.color = vec3(0, 0, 1);
      _drawer.draw(_skeleton_2, *this);

      setColor(vec3(0, 0, 1));
      _spineTarget = _spineOffset + vec3(0, 0, sin(3.0f * elapsedTime()) * 50.0f);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;
   Skeleton _skeleton_2;
   Motion _motion_2;
   vec3 _originalHip;

   // Hand target positions
   vec3 _spineTarget;
   vec3 _spineOffset;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}
