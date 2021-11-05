#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective)
   {
   }

   void setup()
   {
      Joint *body = new Joint("Body");
      body->setLocalTranslation(vec3(1, 2, 1) * 100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0, 1, 0)));
      skeleton.addJoint(body);

      Joint *lwing1 = new Joint("LWing1");
      lwing1->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lwing1, body);

      Joint *lwing2 = new Joint("LWing2");
      lwing2->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lwing2, body);

      Joint *rwing1 = new Joint("RWing1");
      rwing1->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rwing1, body);

      Joint *rwing2 = new Joint("RWing2");
      rwing2->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rwing2, body);

      skeleton.fk();
   }

   void scene()
   {
      Joint *body = skeleton.getByName("Body");

      Joint *lwing1 = skeleton.getByName("LWing1");
      lwing1->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 0, 1)));

      Joint *lwing2 = skeleton.getByName("LWing2");
      lwing2->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 0, 1)));

      Joint *rwing1 = skeleton.getByName("RWing1");
      rwing1->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0, 0, 1)));

      Joint *rwing2 = skeleton.getByName("RWing2");
      rwing2->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0, 0, 1)));
      skeleton.fk();

      // attach geometry to skeleton
      Transform B = body->getLocal2Global();
      Transform LT1 = lwing1->getLocal2Global();
      Transform LT2 = lwing2->getLocal2Global();
      Transform RT1 = rwing1->getLocal2Global();
      Transform RT2 = rwing2->getLocal2Global();

      // draw body
      Transform bodyGeometry(
          glm::angleAxis(glm::pi<float>() * 0.5f, vec3(1, 0, 0)), // rotation
          vec3(0), vec3(25, 200, 25));                            // position, scale

      Transform lwingGeometry1(
          eulerAngleRO(XYZ, vec3(0, M_PI / 4, 0)),
          vec3(-80, 0, -50),
          vec3(100, 20, 150));

      Transform lwingGeometry2(
          eulerAngleRO(XYZ, vec3(0, -M_PI / 4, 0)),
          vec3(-100, 0, 50),
          vec3(120, 20, 200));

      Transform rwingGeometry1(
          eulerAngleRO(XYZ, vec3(0, -M_PI / 4, 0)),
          vec3(80, 0, -50),
          vec3(100, 20, 150));

      Transform rwingGeometry2(
          eulerAngleRO(XYZ, vec3(0, M_PI / 4, 0)),
          vec3(100, 0, 50),
          vec3(120, 20, 200));

      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.8, 0, 0.0));
      push();
      transform(LT1 * lwingGeometry1);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.8, 0, 0.0));
      push();
      transform(LT2 * lwingGeometry2);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RT1 * rwingGeometry1);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RT2 * rwingGeometry2);
      drawSphere(vec3(0), 1);
      pop();

      if (showAxes)
      {
         setColor(vec3(1, 0, 0));
         push();
         transform(B);
         drawEllipsoid(vec3(0), vec3(250, 0, 0), 5);
         pop();

         setColor(vec3(0, 1, 0));
         push();
         transform(B);
         drawEllipsoid(vec3(0), vec3(0, 250, 0), 5);
         pop();

         setColor(vec3(0, 0, 1));
         push();
         transform(B);
         drawEllipsoid(vec3(0), vec3(0, 0, 250), 5);
         pop();
      }
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_SPACE)
      {
         showAxes = !showAxes;
      }
   }

private:
   Skeleton skeleton;
   bool showAxes = false;
};

int main(int argc, char **argv)
{
   Butterfly viewer;
   viewer.run();
}
