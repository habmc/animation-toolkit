#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"

//--------------------------------
// Hermite
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1, 0, 0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const
    {
        glm::vec3 p0 = mCtrlPoints[segment * 2];
        glm::vec3 p1 = mCtrlPoints[segment * 2 + 2];
        glm::vec3 p0Prime = mCtrlPoints[segment * 2 + 1];
        glm::vec3 p1Prime = mCtrlPoints[segment * 2 + 3];
        return (float)(1 - 3 * pow(u, 2) + 2 * pow(u, 3)) * p0 + (float)(u * pow(1 - u, 2)) * p0Prime + (float)(pow(u, 3) - pow(u, 2)) * p1Prime + (float)(3 * pow(u, 2) - 2 * pow(u, 3)) * p1;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
    {
        clearControlPoints();
        int numKeys = keys.size();
        if (numKeys < 2)
        {
            return;
        }
        Eigen::MatrixXd A(numKeys, numKeys);
        Eigen::MatrixXd p(numKeys, 3);
        Eigen::MatrixXd pPrime(numKeys, 3); // slopes for each control point

        if (isClamped())
        {
            A(0, 0) = 1.0f;
            for (int i = 1; i < numKeys; i++)
            {
                A(0, i) = 0.0f;
            }
            for (int i = 0; i < numKeys - 1; i++)
            {
                A(numKeys - 1, i) = 0.0f;
            }
            A(numKeys - 1, numKeys - 1) = 1;
            p(0, 0) = 1.0f;
            p(0, 1) = 0.0f;
            p(0, 2) = 0.0f;
            p(numKeys - 1, 0) = 1.0f;
            p(numKeys - 1, 1) = 0.0f;
            p(numKeys - 1, 2) = 0.0f;
        }
        else
        {
            p(0, 0) = (keys[1][0] - keys[0][0]) * 3.0f;
            p(0, 1) = (keys[1][1] - keys[0][1]) * 3.0f;
            p(0, 2) = 0.0f;
            p(numKeys - 1, 0) = (keys[numKeys - 1][0] - keys[numKeys - 2][0]) * 3.0f;
            p(numKeys - 1, 1) = (keys[numKeys - 1][1] - keys[numKeys - 2][1]) * 3.0f;
            p(numKeys - 1, 2) = 0.0f;

            A(0, 0) = 2.0f;
            A(0, 1) = 1.0f;
            for (int i = 2; i < numKeys; i++)
            {
                A(0, i) = 0.0f;
            }
            for (int i = 0; i < numKeys - 2; i++)
            {
                A(numKeys - 1, i) = 0.0f;
            }
            A(numKeys - 1, numKeys - 2) = 1.0f;
            A(numKeys - 1, numKeys - 1) = 2.0f;
        }

        for (int i = 1; i < numKeys - 1; i++)
        {
            for (int j = 0; j < numKeys; j++)
            {
                if (j == i - 1)
                {
                    A(i, j) = 1.0f;
                }
                else if (j == i)
                {
                    A(i, j) = 4.0f;
                }
                else if (j == i + 1)
                {
                    A(i, j) = 1.0f;
                }
                else
                {
                    A(i, j) = 0.0f;
                }
            }
        }
        for (int i = 1; i < numKeys - 1; i++)
        {
            p(i, 0) = (keys[i + 1][0] - keys[i - 1][0]) * 3.0f;
            p(i, 1) = (keys[i + 1][1] - keys[i - 1][1]) * 3.0f;
            p(i, 2) = (keys[i + 1][2] - keys[i - 1][2]) * 3.0f;
        }

        pPrime = A.inverse() * p;
        int pIndex = 0;
        while (pIndex < numKeys)
        {
            mCtrlPoints.push_back(keys[pIndex]);
            mCtrlPoints.push_back(glm::vec3(pPrime(pIndex, 0), pPrime(pIndex, 1), pPrime(pIndex, 2)));
            pIndex++;
        }
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3 &v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
};

#endif
