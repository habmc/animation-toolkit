#include "atkui/framework.h"
#include "agl/window.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
  }
  virtual void setUp() {
    vec3 currentPosition = vec3(0, 0, 0);
    vec3 velocity = vec3(0, 0, 0);
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of the world
    float radius = 50.0;
    currentPosition = currentPosition + velocity * dt();
    drawSphere(vec3(currentPosition), radius);
  }

  virtual void keyUp(int key, int mods) {
    if (key == GLFW_KEY_SPACE) {
      // update the velocity to move the sphere in a random direction
      velocity = agl::randomUnitVector() * -40.0f;
    } else if (key == GLFW_KEY_R) {
      // reset the position of the sphere and velocity to (0,0,0)
      currentPosition = vec3(0,0,0);
      velocity = vec3(0,0,0);
    }
  }

  vec3 currentPosition;
  vec3 velocity;
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
