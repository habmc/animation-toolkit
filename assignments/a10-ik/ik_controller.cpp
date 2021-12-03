#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton &skeleton,
                                   int jointid, const vec3 &goalPos, float epsilon)
{
  if (jointid == -1)
    return true;

  Joint *ankle = skeleton.getByID(jointid);
  if (!ankle->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *knee = ankle->getParent();
  if (!knee->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *hip = knee->getParent();

  // TODO: Your code here
  Joint *hipParent = hip->getParent();

  float l1 = length(hip->getGlobalTranslation() - knee->getGlobalTranslation());
  float l2 = length(knee->getGlobalTranslation() - hip->getGlobalTranslation());
  float r = length(goalPos - hip->getGlobalTranslation());
  float cosPhi = (pow(r, 2) - pow(l1, 2) - pow(l2, 2)) / (-2.0f * l1 * l2);
  cosPhi = glm::clamp(cosPhi, -1.0f, 1.0f);
  float phi = acos(cosPhi);
  float theta2z = phi - M_PI;

  vec3 limbDir = normalize(knee->getLocalTranslation());
  vec3 axis = cross(limbDir, vec3(0, 0, -1));
  if (limbDir[1] < 0)
    axis = cross(limbDir, vec3(0, 0, 1));

  Transform F21 = Transform(glm::angleAxis(theta2z, axis), knee->getLocalTranslation());
  knee->setLocal2Parent(F21);
  hip->setLocalRotation(IdentityQ);
  skeleton.fk();

  vec3 r0 = ankle->getGlobalTranslation() - hip->getGlobalTranslation();
  vec3 e0 = goalPos - ankle->getGlobalTranslation();
  float phi2 = atan2(length(cross(r0, e0)), dot(r0, r0) + dot(r0, e0));
  vec3 u0 = normalize(cross(r0, e0));

  vec3 rotAxis = (hip != skeleton.getRoot()) ? hipParent->getLocal2Global().inverse().transformVector(u0) : u0;

  Transform F10 = Transform::Rot(angleAxis(phi2, rotAxis));

  hip->setLocal2Parent(hip->getLocal2Parent() * F10);
  skeleton.fk();

  if (length(goalPos - ankle->getGlobalTranslation()) <= epsilon)
    return true;
  return false;
}

bool IKController::solveIKCCD(Skeleton &skeleton, int jointid,
                              const vec3 &goalPos, const std::vector<Joint *> &chain,
                              float threshold, int maxIters, float nudgeFactor)
{
  // There are no joints in the IK chain for manipulation
  if (chain.size() == 0)
    return true;

  // TODO: Your code here
  Joint *endEffector = skeleton.getByID(jointid);
  int iter = 0;
  while (length(goalPos - endEffector->getGlobalTranslation()) > threshold && iter < maxIters)
  {
    for (int i = 0; i < chain.size(); i++)
    {
      Joint *curr = chain[i];
      vec3 r0 = endEffector->getGlobalTranslation() - curr->getGlobalTranslation();
      vec3 e0 = goalPos - endEffector->getGlobalTranslation();
      float phi2 = atan2(length(cross(r0, e0)), dot(r0, r0) + dot(r0, e0));
      vec3 u0 = normalize(cross(r0, e0));
      u0 = (curr->getParent() != skeleton.getRoot()) ? curr->getParent()->getLocal2Global().inverse().transformVector(u0) : u0;

      if (length(u0) > 0)
      {
        Transform F = Transform::Rot(glm::angleAxis((float)phi2 * nudgeFactor, u0));
        curr->setLocal2Parent(curr->getLocal2Parent() * F);
        skeleton.fk();
      }
    }
    iter++;
  }
  if (length(goalPos - endEffector->getGlobalTranslation()) <= threshold)
    return true;
  return false;
}
