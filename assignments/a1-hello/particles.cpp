#include "atkui/framework.h"

using namespace glm;

class Particles : public atkui::Framework
{

  struct Particle
  {
    vec3 position;
    vec3 color;
    float velocity;
  };

public:
  Particles() : atkui::Framework(atkui::Orthographic)
  {
  }

  virtual void setup()
  {
    vec3 baseColor = vec3(0.6, 0.4, 0.6);
    for (int i = 0; i < sizeof(particles) / sizeof(particles[0]); i++)
    {
      Particle p;
      p.position = vec3(rand() % (int)width(), rand() % (int)height(), 0);
      float minColorChange = -0.2f;
      float maxColorChange = 0.2f;
      float r1 = minColorChange + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxColorChange - minColorChange)));
      float r2 = minColorChange + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxColorChange - minColorChange)));
      float r3 = minColorChange + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxColorChange - minColorChange)));
      p.color = baseColor - vec3(r1, r2, r3);
      int minVelocity = 50;
      int maxVelocity = 120;
      p.velocity = rand() % (maxVelocity - minVelocity + 1) + minVelocity;
      particles[i] = p;
    }
  }

  virtual void scene()
  {
    int n = sizeof(particles) / sizeof(particles[0]);
    for (int i = 0; i < n; i++)
    {
      particles[i].position[0] = particles[i].position[0] + dt() * particles[i].velocity;
      particles[i].position[1] = particles[i].position[1] + dt() * particles[i].velocity;

      if (particles[i].position[0] >= width())
      {
        particles[i].position[0] = 0;
      }
      if (particles[i].position[1] >= height())
      {
        particles[i].position[1] = 0;
      }
      setColor(particles[i].color);
      drawSphere(particles[i].position, 10);
    }
  }

private:
  Particle particles[500];
};

int main(int argc, char **argv)
{
  Particles viewer;
  viewer.run();
  return 0;
}
