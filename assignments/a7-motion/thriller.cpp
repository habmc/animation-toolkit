#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup()
   {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 position = vec3(0);
      vec3 color = vec3(1, 0, 0);
      float size = 1.0f;
      int distance = 200;
      for (int row = 0; row < 3; row++)
      {
         for (int col = 0; col < 4; col++)
         {
            position = vec3(row * distance + agl::random() * 25 - 200, 0, col * distance + agl::random() * 25 - 200);
            color = agl::random() * vec3(1, 0, 0);
            size = agl::random(0.5f, 0.75f);
            devils.push_back(Devil(position, color, size));
         }
      }
   }

   virtual void scene()
   {
      if (!_paused)
         _motion.update(_skeleton, elapsedTime());
      for (int i = 0; i < devils.size(); i++)
      {
         devils[i].draw(_skeleton, *this);
      }
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == 'P')
         _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   std::vector<Devil> devils;
};

int main(int argc, char **argv)
{
   Thriller viewer;
   viewer.run();
}
