#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
   WASDSimple() : atkui::Framework(atkui::Perspective)
   {
   }

   virtual void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton_1, _walk_1);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton_2, _walk_2);
      for (int i = 0; i < _walk_1.getNumKeys(); i++)
      {
         Pose newPose = _walk_1.getKey(i);
         newPose.rootPos = vec3(_walk_1.getKey(i).rootPos[0], _walk_1.getKey(i).rootPos[1], _walk_1.getKey(i).rootPos[2] - 100);
         _walk_1.editKey(i, newPose);
      }
      for (int i = 0; i < _walk_2.getNumKeys(); i++)
      {
         Pose newPose = _walk_2.getKey(i);
         newPose.rootPos = vec3(_walk_2.getKey(i).rootPos[0], _walk_2.getKey(i).rootPos[1], _walk_2.getKey(i).rootPos[2] + 100);
         newPose.jointRots[0] = glm::angleAxis((float)M_PI, vec3(0, 1, 0));
         _walk_2.editKey(i, newPose);
      }
   }

   virtual void scene()
   {
      lookAt(vec3(-500, 500, -500), vec3(0, 0, 0), vec3(0, 1, 0));
      _walk_1.update(_skeleton_1, elapsedTime());
      _drawer.color = vec3(1);
      _drawer.draw(_skeleton_1, *this);
      _walk_2.update(_skeleton_2, elapsedTime());
      _drawer.color = vec3(0);
      _drawer.draw(_skeleton_2, *this);
      vec3 color = discoPallet[rand() % 5];
      setColor(color);
      drawCube(vec3(0), vec3(2000, 1, 2000));
      drawSphere(vec3(100, 350, 0), 50);
   }

protected:
   Motion _walk_1;
   Motion _walk_2;
   Skeleton _skeleton_1;
   Skeleton _skeleton_2;
   atkui::SkeletonDrawer _drawer;
   std::vector<vec3> discoPallet =
       {
           vec3(15, 192, 252) / 255.0f,
           vec3(123, 29, 175) / 255.0f,
           vec3(255, 47, 185) / 255.0f,
           vec3(212, 255, 71) / 255.0f,
           vec3(27, 54, 73) / 255.0f};
};

int main(int argc, char **argv)
{
   WASDSimple viewer;
   viewer.run();
}
