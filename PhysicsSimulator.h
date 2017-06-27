#ifndef PHYSICSSIMULATOR_H
#define PHYSICSSIMULATOR_H

#include "btBulletDynamicsCommon.h"

class Controller;

class PhysicsSimulator
{
public:
    PhysicsSimulator(Controller *c);
    ~PhysicsSimulator();
    void simSetup();
    void simStep();
    void simClose();

private:
    Controller *ctrl;

    // Bullet instance objects.
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    btCollisionShape* groundShape;
    btTransform groundTransform;

    btScalar mass;
    bool isDynamic;

    btVector3 localInertia;

    btDefaultMotionState* myMotionState;
    btRigidBody* body;

    btCollisionShape* colShape;

    btTransform startTransform;
};

#endif //PHYSICSSIMULATOR_H


