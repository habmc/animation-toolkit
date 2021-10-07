#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

namespace atkmath
{
   Matrix3 getRx(double alpha)
   {
      Matrix3 Rx;
      Rx[0][0] = 1;
      Rx[0][1] = 0;
      Rx[0][2] = 0;
      Rx[1][0] = 0;
      Rx[1][1] = cos(alpha);
      Rx[1][2] = -sin(alpha);
      Rx[2][0] = 0;
      Rx[2][1] = sin(alpha);
      Rx[2][2] = cos(alpha);
      return Rx;
   }

   Matrix3 getRy(double theta)
   {
      Matrix3 Ry;
      Ry[0][0] = cos(theta);
      Ry[0][1] = 0;
      Ry[0][2] = sin(theta);
      Ry[1][0] = 0;
      Ry[1][1] = 1;
      Ry[1][2] = 0;
      Ry[2][0] = -sin(theta);
      Ry[2][1] = 0;
      Ry[2][2] = cos(theta);
      return Ry;
   }

   Matrix3 getRz(double beta)
   {
      Matrix3 Rz;
      Rz[0][0] = cos(beta);
      Rz[0][1] = -sin(beta);
      Rz[0][2] = 0;
      Rz[1][0] = sin(beta);
      Rz[1][1] = cos(beta);
      Rz[1][2] = 0;
      Rz[2][0] = 0;
      Rz[2][1] = 0;
      Rz[2][2] = 1;
      return Rz;
   }

   Vector3 Matrix3::toEulerAnglesXYZ() const
   {
      return Vector3();
   }

   Vector3 Matrix3::toEulerAnglesXZY() const
   {
      return Vector3();
   }

   Vector3 Matrix3::toEulerAnglesYXZ() const
   {
      return Vector3();
   }

   Vector3 Matrix3::toEulerAnglesYZX() const
   {
      return Vector3();
   }

   Vector3 Matrix3::toEulerAnglesZXY() const
   {
      return Vector3();
   }

   Vector3 Matrix3::toEulerAnglesZYX() const
   {
      return Vector3();
   }

   void Matrix3::fromEulerAnglesXYZ(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];
      Matrix3 Rx = getRx(alpha);
      Matrix3 Ry = getRy(theta);
      Matrix3 Rz = getRz(beta);
      *this = Rx * Ry * Rz;
   }

   void Matrix3::fromEulerAnglesXZY(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];
      Matrix3 Rx = getRx(alpha);
      Matrix3 Ry = getRy(theta);
      Matrix3 Rz = getRz(beta);
      *this = Rx * Rz * Ry;
   }

   void Matrix3::fromEulerAnglesYXZ(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];
      Matrix3 Rx = getRx(alpha);
      Matrix3 Ry = getRy(theta);
      Matrix3 Rz = getRz(beta);
      *this = Ry * Rx * Rz;
   }

   void Matrix3::fromEulerAnglesYZX(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];
      Matrix3 Rx = getRx(alpha);
      Matrix3 Ry = getRy(theta);
      Matrix3 Rz = getRz(beta);
      *this = Ry * Rz * Rx;
   }

   void Matrix3::fromEulerAnglesZXY(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];
      Matrix3 Rx = getRx(alpha);
      Matrix3 Ry = getRy(theta);
      Matrix3 Rz = getRz(beta);
      *this = Rz * Rx * Ry;
   }

   void Matrix3::fromEulerAnglesZYX(const Vector3& angleRad)
   {
      double alpha = angleRad[0];
      double theta = angleRad[1];
      double beta = angleRad[2];

      Matrix3 Rx = getRx(angleRad[0]);
      Matrix3 Ry = getRy(angleRad[1]);
      Matrix3 Rz = getRz(angleRad[2]);
      *this = Rz * Ry * Rx;
   }

   void Matrix3::toAxisAngle(Vector3& axis, double& angleRad) const
   {
      // TODO
   }

   void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
   {
      // TODO
      // *this = Identity;
   }
}