#include "atkui/framework.h"
#include "atkmath/matrix3.h"
#include <Eigen/Dense>
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }
   virtual void setup()
   {
      int radius = 50;
      int sideLength = 15;
      vec3 center = vec3(width() / 2, height() / 2, 0);
      for (int j = 0; j < 8; j++)
      {
         colors[j] = pallet[j % 5];
         radiusArray[j] = radius;
         for (int i = 0; i < 20; i++)
         {
            long double theta = atkmath::PI * 18 * i / 180;
            long double p_x = center[0] + cos(theta) * radius;
            long double p_y = center[1] + sin(theta) * radius;
            firstPoints[j][i] = vec3(p_x, p_y, 0);
            secondPoints[j][i] = vec3(p_x, p_y + sideLength, 0);
            thirdPoints[j][i] = vec3(p_x + sideLength, p_y, 0);
         }
         radius += 50;
         sideLength += 5;
      }
   }

   virtual void scene()
   {
      euler[2] += 2;
      euler[2] = fmod(euler[2], 360);
      rot.fromEulerAnglesXYZ(euler * Deg2Rad);
      for (int j = 0; j < 8; j++)
      {
         if (euler[2] == 0)
         {
            colors[j] = pallet[rand() % 5]; // change color after each 360deg rotation
         }
         setColor(colors[j]);
         for (int i = 0; i < 20; i++)
         {
            Eigen::MatrixXd rot2 = matrix3ToMatrix(rot);
            Eigen::MatrixXd matrix2 = vec3ToMatrix(vec3(secondPoints[j][i][0] - firstPoints[j][i][0], secondPoints[j][i][1] - firstPoints[j][i][1], 0));
            Eigen::MatrixXd matrix3 = vec3ToMatrix(vec3(thirdPoints[j][i][0] - firstPoints[j][i][0], thirdPoints[j][i][1] - firstPoints[j][i][1], 0));
            matrix2 = rot2 * matrix2;
            matrix3 = rot2 * matrix3;
            drawTriangle(firstPoints[j][i], vec3(firstPoints[j][i][0] + matrix2(0, 0), firstPoints[j][i][1] + matrix2(1, 0), matrix2(2, 0)), vec3(firstPoints[j][i][0] + matrix3(0, 0), firstPoints[j][i][1] + matrix3(1, 0), matrix3(2, 0)));
         }
      }
   }

   virtual Eigen::MatrixXd vec3ToMatrix(vec3 vec)
   {
      Eigen::MatrixXd M(3, 1);
      M(0, 0) = vec[0];
      M(1, 0) = vec[1];
      M(2, 0) = vec[2];
      return M;
   }

   virtual Eigen::MatrixXd matrix3ToMatrix(atkmath::Matrix3 m)
   {
      Eigen::MatrixXd M(3, 3);
      M(0, 0) = m[0][0];
      M(0, 1) = m[0][1];
      M(0, 2) = m[0][2];
      M(1, 0) = m[1][0];
      M(1, 1) = m[1][1];
      M(1, 2) = m[1][2];
      M(2, 0) = m[2][0];
      M(2, 1) = m[2][1];
      M(2, 2) = m[2][2];
      return M;
   }

   virtual void drawTriangle(vec3 first, vec3 second, vec3 third)
   {
      drawLine(first, second);
      drawLine(second, third);
      drawLine(third, first);
   }

private:
   vec3 firstPoints[8][20];
   vec3 secondPoints[8][20];
   vec3 thirdPoints[8][20];
   int radiusArray[8];
   vec3 colors[8];
   std::vector<vec3> pallet =
       {
           vec3(0, 165, 227) / 255.0f,
           vec3(141, 215, 191) / 255.0f,
           vec3(255, 150, 197) / 255.0f,
           vec3(255, 87, 104) / 255.0f,
           vec3(255, 162, 58) / 255.0f};
   atkmath::Matrix3 rot;
   atkmath::Vector3 euler = atkmath::Vector3(0, 0, 0);
   const double Deg2Rad = (atkmath::PI / 180.0f);
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}