#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath
{

	Quaternion Quaternion::Slerp(const Quaternion &q0, const Quaternion &q1, double t)
	{
		// TODO
		return Quaternion(1, 0, 0, 0);
	}

	void Quaternion::toAxisAngle(Vector3 &axis, double &angleRad) const
	{
		angleRad = acos(mW) * 2;
		axis = Vector3(mX / sin(angleRad * 0.5), mY / sin(angleRad * 0.5), mZ / sin(angleRad * 0.5));
	}

	void Quaternion::fromAxisAngle(const Vector3 &axis, double angleRad)
	{
		mX = sin(angleRad * 0.5) * axis.x();
		mY = sin(angleRad * 0.5) * axis.y();
		mZ = sin(angleRad * 0.5) * axis.z();
		mW = cos(angleRad * 0.5);
		normalize();
	}

	Matrix3 Quaternion::toMatrix() const
	{
		Matrix3 M;
		double xx = mX * mX;
		double xy = mX * mY;
		double xz = mX * mZ;
		double xw = mX * mW;
		double yy = mY * mY;
		double yz = mY * mZ;
		double yw = mY * mW;

		double zz = mZ * mZ;
		double zw = mZ * mW;

		M[0][0] = 1 - 2 * (yy + zz);
		M[0][1] = 2 * (xy - zw);
		M[0][2] = 2 * (xz + yw);

		M[1][0] = 2 * (xy + zw);
		M[1][1] = 1 - 2 * (xx + zz);
		M[1][2] = 2 * (yz - xw);

		M[2][0] = 2 * (xz - yw);
		M[2][1] = 2 * (yz + xw);
		M[2][2] = 1 - 2 * (xx + yy);
		return M;
	}

	void Quaternion::fromMatrix(const Matrix3 &rot)
	{
		double wSquare = (1 + rot[0][0] + rot[1][1] + rot[2][2]) / 4;
		double vxSquare = (1 + rot[0][0] - rot[1][1] - rot[2][2]) / 4;
		double vySquare = (1 + rot[1][1] - rot[0][0] - rot[2][2]) / 4;
		double vzSquare = (1 + rot[2][2] - rot[0][0] - rot[1][1]) / 4;
		double max = std::max(std::max(std::max(wSquare, vxSquare), vySquare), vzSquare);
		if (wSquare == max)
		{
			mW = sqrt(wSquare);
			mZ = (rot[1][0] - rot[0][1]) / (4 * mW);
			mX = (rot[2][1] - rot[1][2]) / (4 * mW);
			mY = (rot[0][2] - rot[2][0]) / (4 * mW);
		}
		else if (vxSquare == max)
		{
			mX = sqrt(vxSquare);
			mW = (rot[2][1] - rot[1][2]) / (4 * mX);
			mY = (rot[0][2] - rot[2][0]) / (4 * mW);
			mZ = (rot[1][0] - rot[0][1]) / (4 * mW);
		}
		else if (vySquare == max)
		{
			mY = sqrt(vySquare);
			mW = (rot[0][2] - rot[2][0]) / (4 * mY);
			mX = (rot[2][1] - rot[1][2]) / (4 * mW);
			mZ = (rot[1][0] - rot[0][1]) / (4 * mW);
		}
		else
		{
			mZ = sqrt(vzSquare);
			mW = (rot[1][0] - rot[0][1]) / (4 * mZ);
			mX = (rot[2][1] - rot[1][2]) / (4 * mW);
			mY = (rot[0][2] - rot[2][0]) / (4 * mW);
		}
	}

}