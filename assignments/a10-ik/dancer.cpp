#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _drawer.color = vec3(1, 0, 0);
      _motion.update(_skeleton, 0);

      _lhandOffset = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation() + vec3(0, 50, 15);
      _rhandOffset = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation() + vec3(0, 50, 15);
      _originalHip = _skeleton.getRoot()->getGlobalTranslation();
      _lfootOriginal = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();
      _rfootOriginal = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
      _lfootOriginalRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
      _rfootOriginalRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());

      IKController ik;
      // TODO: Your code here
      _skeleton.getRoot()->setLocalTranslation(_originalHip + vec3(sin(3.0f * elapsedTime()) * 5.0f, abs(cos(3.0f * elapsedTime()) * 5.0f), sin(3.0f * elapsedTime() * 3.0f)));
      _skeleton.fk();

      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftHand")->getID(), _lhandTarget, 0.001f);
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightHand")->getID(), _rhandTarget, 0.001f);

      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftFoot")->getID(), _lfootOriginal, 0.001f);
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightFoot")->getID(), _rfootOriginal, 0.001f);

      _skeleton.getByName("Beta:LeftFoot")->setLocalRotation(_skeleton.getByName("Beta:LeftFoot")->getLocalRotation() * inverse(_skeleton.getByName("Beta:LeftFoot")->getGlobalRotation()) * _lfootOriginalRot);
      _skeleton.getByName("Beta:RightFoot")->setLocalRotation(_skeleton.getByName("Beta:RightFoot")->getLocalRotation() * inverse(_skeleton.getByName("Beta:RightFoot")->getGlobalRotation()) * _rfootOriginalRot);

      _skeleton.fk();
   }

   void scene()
   {
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0, 0, 1));
      _lhandTarget = _lhandOffset + vec3(0, sin(3.0f * elapsedTime()) * 50.0f, 0);
      _rhandTarget = _rhandOffset + vec3(0, sin(3.0f * elapsedTime() + M_PI) * 50.0f, 0);

      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;
   vec3 _originalHip;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;
   vec3 _lhandOffset;
   vec3 _rhandOffset;
   vec3 _lfootOriginal;
   vec3 _rfootOriginal;

   quat _lfootOriginalRot;
   quat _rfootOriginalRot;
};

int main(int argc, char **argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}
