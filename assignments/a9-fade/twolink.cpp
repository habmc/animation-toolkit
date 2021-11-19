#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "atk/toolkit.h"
#include "atkui/skeleton_drawer.h"
#include "atkui/framework.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

using namespace atk;
using namespace glm;

class AIKSimple : public atkui::Framework
{
public:
  AIKSimple() : atkui::Framework(atkui::Perspective),
                mDrawer(),
                mGoalPosition()
  {
    mDrawer.showAxes = true;
    mDrawer.color = vec3(0.6, 1.0, 0.4);
  }

  virtual ~AIKSimple()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void setup()
  {
    Joint *shoulder = new Joint("Shoulder");
    mActor.addJoint(shoulder);
    shoulder->setLocalTranslation(vec3(0, 0, 0));

    Joint *elbow = new Joint("Elbow");
    mActor.addJoint(elbow, shoulder);
    elbow->setLocalTranslation(vec3(100, 0, 0));

    Joint *wrist = new Joint("Wrist");
    mActor.addJoint(wrist, elbow);
    wrist->setLocalTranslation(vec3(80, 0, 0));

    mActor.fk();
    mGoalPosition = wrist->getGlobalTranslation();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
#endif
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void drawGui()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Demo Controls");
    ImGui::SetWindowSize(ImVec2(150, 150));
    ImGui::SetWindowPos(ImVec2(5, 5));
    ImGui::SliderFloat("X", &mGoalPosition[0], -500.0f, 500.0f);
    ImGui::SliderFloat("Y", &mGoalPosition[1], -500.0f, 500.0f);
    ImGui::SliderFloat("Z", &mGoalPosition[2], -500.0f, 500.0f);
    if (ImGui::Button("Reset"))
      reset();
    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void reset()
  {
    for (int i = 0; i < mActor.getNumJoints(); i++)
    {
      mActor.getByID(i)->setLocalRotation(atk::IdentityQ);
    }
    mActor.fk();
    mGoalPosition = mActor.getByID(2)->getGlobalTranslation();
  }

  void draw()
  {
    mDrawer.draw(mActor, *this);
    drawFloor(2000, 20, 50);

    vec2 screenPos = worldToScreen(mGoalPosition);

    setColor(vec3(1.0, 0.0, 1.0));
    ortho(0, width(), 0, height(), -1000, 1000);
    renderer.lookAt(vec3(0, 0, 1), vec3(0), vec3(0, 1, 0));

    vec3 c = vec3(screenPos, 0);
    vec3 v1 = c + vec3(10, 0, 0);
    vec3 v2 = c - vec3(10, 0, 0);
    vec3 h1 = c + vec3(0, 10, 0);
    vec3 h2 = c - vec3(0, 10, 0);

    beginShader("unlit");
    drawCircle(c, 5.0f);
    drawLine(v1, v2);
    drawLine(h1, h2);
    endShader();

    // reset projection
    perspective(glm::radians(60.0f), width() / (float)height(), 10.0f, 2000.0f);
    renderer.lookAt(camera.position(), camera.look(), camera.up());

    drawGui();
    solveIKTwoLink(mActor, mGoalPosition);
  }

  void solveIKTwoLink(Skeleton &skeleton, const vec3 &goalPosition)
  {
    // todo: implement two link IK algorithm
    float l1 = length(skeleton.getByName("Elbow")->getGlobalTranslation() - skeleton.getByName("Shoulder")->getGlobalTranslation());
    float l2 = length(skeleton.getByName("Wrist")->getGlobalTranslation() - skeleton.getByName("Elbow")->getGlobalTranslation());
    float r = length(goalPosition);

    float cosFi = (pow(r, 2) - pow(l1, 2) - pow(l2, 2)) / (-2 * l1 * l2);
    cosFi = min(max(cosFi, -1.0f), 1.0f);
    float fi = acos(cosFi);

    float theta2z = fi - pi<float>();

    float sinTheta1z = (-1 * l2 * sin(theta2z)) / r;
    sinTheta1z = min(max(sinTheta1z, -1.0f), 1.0f);
    float theta1z = asin(sinTheta1z);

    float gamma = asin(goalPosition[1] / r);
    float beta = atan2(-1 * goalPosition[2], goalPosition[0]);

    quat Q21 = angleAxis(theta2z, vec3(0, 0, 1));
    quat RyBeta = angleAxis(beta, vec3(0, 1, 0));
    quat RzGamma = angleAxis(gamma, vec3(0, 0, 1));
    quat RzTheta1Z = angleAxis(theta1z, vec3(0, 0, 1));

    quat Q10 = RyBeta * RzGamma * RzTheta1Z;

    skeleton.getByName("Elbow")->setLocalRotation(Q21);
    skeleton.getByName("Shoulder")->setLocalRotation(Q10);
    skeleton.getRoot()->fk();
  }

  mat3 Rx(const float &theta)
  {
    return mat3(
        1, 0, 0,
        0, cos(theta), -sin(theta),
        0, sin(theta), cos(theta));
  }

  mat3 Ry(const float &theta)
  {
    return mat3(
        cos(theta), 0, sin(theta),
        0, 1, 0,
        -sin(theta), 0, cos(theta));
  }

  mat3 Rz(const float &theta)
  {
    return mat3(
        cos(theta), -sin(theta), 0,
        sin(theta), cos(theta), 0,
        0, 0, 1);
  }

private:
  atk::Skeleton mActor;
  atkui::SkeletonDrawer mDrawer;
  glm::vec3 mGoalPosition;
};

int main(int argc, char **argv)
{
  AIKSimple viewer;
  viewer.run();
  return 0;
}
