#include "steerable.h"
#include "behaviors.h"

using namespace glm;
using namespace atk;

float ASteerable::kVelKv = 150.0;
float ASteerable::kOriKv = 16.0;
float ASteerable::kOriKp = 64.0;

// Given a desired velocity, veld, and dt, compute a transform holding
// the new orientation and change in position
//
// rotation control: tau = I * (-Kv1 * thetaDot + Kp1 * (thetaD - theta))
// translation control: f = m * Kv0 * (vd - v)
void ASteerable::senseControlAct(const vec3 &veld, float dt)
{
   // Compute _vd and _thetad
   _vd = length(veld);
   _thetad = atan2(veld[0], veld[2]);

   // compute _force and _torque
   _force = _mass * kVelKv * (_vd - _state[VEL]);
   _torque = _inertia * (-kOriKv * _state[AVEL] + kOriKp * (fmod(_thetad - _state[ORI] + M_PI, M_PI * 2) - M_PI));

   // find derivative
   _derivative[0] = _state[VEL];
   _derivative[1] = _state[AVEL];
   _derivative[2] = _force / _mass;
   _derivative[3] = _torque / _inertia;

   // update state
   _state[POS] += dt * _derivative[0];
   _state[ORI] += dt * _derivative[1];
   _state[VEL] += dt * _derivative[2];
   _state[AVEL] += dt * _derivative[3];

   // compute global position and orientation and update _characterRoot
   quat rot = glm::angleAxis(_state[ORI], vec3(0, 1, 0));
   vec3 localPos(0, 0, _state[POS]);

   _characterRoot.setT(rot * localPos + _characterRoot.t());
   _characterRoot.setR(rot);
}

// randomize the colors, characters, and animation times here
void ASteerable::randomizeAppearance()
{
   // to randomize the walking animations, compute different initial values
   // for _time

   // to randomize color, call _drawer.setColor
   float r1 = -0.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
   float r2 = -0.2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
   float r3 = -0.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6));
   vec3 color = vec3(0.7, 0.4, 0.6) + vec3(r1, r2, r3);
   _drawer.color = color;
   // to randomize shape, compute random values for _drawer.setJointRadius
   _drawer.jointRadius = 5 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 15));

   // or randomly assign different drawers to have a mix of characters
}
