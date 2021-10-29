#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::quat;
using glm::vec3;
class Unique : public atkui::Framework
{
public:
    Unique() : atkui::Framework(atkui::Perspective) {}
    virtual ~Unique() {}

    virtual void setup()
    {
        lookAt(vec3(-20, 50, -700), vec3(0));
        float spacing = 40;
        for (int i = 0; i < 10; i++)
        {
            Skeleton _tentacle;
            Joint *root = new Joint("hip");
            root->setLocalTranslation(vec3(spacing * i - 200, 50, -i * spacing));
            _tentacle.addJoint(root);

            Joint *joint1 = new Joint("right knee");
            joint1->setLocalTranslation(vec3(-20, -20, 0));
            _tentacle.addJoint(joint1, root);

            Joint *joint10 = new Joint("right ankle");
            joint10->setLocalTranslation(vec3(0, -30, 0));
            _tentacle.addJoint(joint10, joint1);

            Joint *joint2 = new Joint("left knee");
            joint2->setLocalTranslation(vec3(20, -20, 0));
            _tentacle.addJoint(joint2, root);

            Joint *joint20 = new Joint("left ankle");
            joint20->setLocalTranslation(vec3(0, -30, 0));
            _tentacle.addJoint(joint20, joint2);

            Joint *joint3 = new Joint("sternum");
            joint3->setLocalTranslation(vec3(0, 40, 0));
            _tentacle.addJoint(joint3, root);

            Joint *joint4 = new Joint("head");
            joint4->setLocalTranslation(vec3(0, 25, 0));
            _tentacle.addJoint(joint4, joint3);

            Joint *joint5 = new Joint("left shoulder");
            joint5->setLocalTranslation(vec3(-20, 5, 0));
            _tentacle.addJoint(joint5, joint3);

            Joint *joint50 = new Joint("left elbow");
            joint50->setLocalTranslation(vec3(-10, -20, 0));
            _tentacle.addJoint(joint50, joint5);

            Joint *joint6 = new Joint("right shoulder");
            joint6->setLocalTranslation(vec3(20, 5, 0));
            _tentacle.addJoint(joint6, joint3);

            Joint *joint60 = new Joint("right elbow");
            joint60->setLocalTranslation(vec3(10, -20, 0));
            _tentacle.addJoint(joint60, joint6);
            _tentacle.fk(); // compute local2global transforms
            _tentacles.push_back(_tentacle);
        }
    }

    virtual void scene()
    {
        push();
        scale(vec3(10));
        vec3 d10 = vec3(-10, 0, -30);
        quat R10 = glm::angleAxis(0.f, vec3(1, 0, 0));
        Transform F10(R10, d10); // transform from frame 1 to world
        transform(F10);
        setColor(discoPallet[rand() % 5]);
        drawCube(vec3(0), vec3(2.5, 5, 5));
        pop();
        for (int j = 0; j < 10; j++)
        {
            Skeleton _tentacle = _tentacles[j];
            int numJoints = _tentacle.getNumJoints();
            setColor(pallet[j % 5]);
            for (int i = 0; i < numJoints; i++)
            {
                _tentacle.fk();
                _tentacle.getByID(7)->setLocalRotation(glm::angleAxis((sin(elapsedTime()) * 25.0f), vec3(0, 1, 0)));
                _tentacle.getByID(9)->setLocalRotation(glm::angleAxis((sin(elapsedTime()) * 25.0f), vec3(0, 1, 0)));
                Joint *joint = _tentacle.getByID(i);
                if (joint->getParent() == 0)
                {
                    continue;
                }
                Joint *parent = joint->getParent();
                vec3 globalParentPos = parent->getGlobalTranslation();
                vec3 globalPos = joint->getGlobalTranslation();
                drawEllipsoid(globalParentPos, globalPos, 7);
            }
        }
    }

protected:
    std::vector<Skeleton> _tentacles;
    std::vector<vec3> pallet =
        {
            vec3(0, 165, 227) / 255.0f,
            vec3(141, 215, 191) / 255.0f,
            vec3(255, 150, 197) / 255.0f,
            vec3(255, 87, 104) / 255.0f,
            vec3(255, 162, 58) / 255.0f};
    std::vector<vec3> discoPallet =
        {
            vec3(15, 192, 252) / 255.0f,
            vec3(123, 29, 175) / 255.0f,
            vec3(255, 47, 185) / 255.0f,
            vec3(212, 255, 71) / 255.0f,
            vec3(27, 54, 73) / 255.0f};
};

int main(int argc, char **argv)
{
    Unique viewer;
    viewer.run();
}
