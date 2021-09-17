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
    for (int i = 0; i < *(&particles + 1) - particles; i++)
    {
      Particle p;
      p.position = vec3(rand() % (int)width(), rand() % (int)height(), rand() % 100);
      vec3 baseColor = vec3(0.2, 0.4, 0.7);
      float r1 = -0.2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.4));
      float r2 = -0.2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.4));
      float r3 = -0.2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.4));
      p.color = baseColor - vec3(r1, r2, r3);
      p.velocity = rand() % (20 - 5 + 1) + 5;
      particles[i] = p;
    }
  }

  virtual void scene()
  {
    for (int i = 0; i < *(&particles + 1) - particles; i++)
    {
      setColor(particles[i].color);
      if (particles[i].position[0] >= width())
      {
        particles[i].position[0] = 0;
      }
      else if (particles[i].position[1] >= height())
      {
        particles[i].position[1] = 0;
      }
      else
      {
        particles[i].position = particles[i].position + vec3(0.1) * particles[i].velocity;
      }
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
