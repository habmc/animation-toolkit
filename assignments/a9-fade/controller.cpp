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
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    _drawer.showAxes = true;
    _pos = _walk.getKey(0).rootPos;
    _pos[1] = 0;
    globalPos = _skeleton.getByName("Beta:Head")->getGlobalTranslation();
    globalLookPos = _skeleton.getByName("Beta:Spine1")->getGlobalTranslation();
  }

  virtual void scene()
  {
    update();
    _drawer.draw(_skeleton, *this);

    // draw heading
    vec3 p = _skeleton.getRoot()->getGlobalTranslation();
    p[1] = 10; // set height close above the floor

    setColor(vec3(0, 1, 1));
    push();
    translate(p);
    rotate(_heading, vec3(0, 1, 0));
    translate(vec3(0, 0, 25));
    scale(vec3(10, 10, 50));
    drawCylinder(vec3(0), 1.0f);
    pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());

    // TODO: Your code here
    Pose pose = _skeleton.getPose();
    pose.rootPos = _pos + vec3(0, pose.rootPos[1], 0);
    pose.jointRots[0] = glm::angleAxis(_heading, vec3(0, 1, 0));
    _skeleton.setPose(pose);

    // TODO: Override the default camera to follow the character
    _pos += vec3(sin(_heading) * speed, 0, cos(_heading) * speed);
    globalPos += vec3(sin(_heading) * speed, 0, cos(_heading) * speed);
    globalLookPos += vec3(sin(_heading) * speed, 0, cos(_heading) * speed);
    vec3 globalCameraPos = globalPos +
                           vec3(-200 * sin(_heading), 200, -200 * cos(_heading));

    lookAt(globalCameraPos, globalLookPos, vec3(0, 1, 0));

    // update heading when key is down
    if (keyIsDown('D'))
      _heading -= 0.05;
    if (keyIsDown('A'))
      _heading += 0.05;
  }

protected:
  float _heading;
  float speed = 0.7;
  vec3 _pos = vec3(0);
  vec3 globalPos;
  vec3 globalLookPos;
  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
