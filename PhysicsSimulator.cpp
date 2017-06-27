// This is based on the Bullet HelloWorld example.
#include "PhysicsSimulator.h"
#include "Controller.h"

PhysicsSimulator::PhysicsSimulator(Controller *c) : ctrl(c)
{
}

//----------------------------------------------------
PhysicsSimulator::~PhysicsSimulator()
{
    //Make sure simClose() for cleaning up is called at some point after simulation.
}

//----------------------------------------------------
void PhysicsSimulator::simSetup()
{
///-----initialization_start-----
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
///-----initialization_end-----

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    //btAlignedObjectArray<btCollisionShape*> collisionShapes;

    ///create a few basic rigid bodies

    //the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5
    {
        groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        collisionShapes.push_back(groundShape);

        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -54, 0));

        //btScalar mass(0.);
        mass = 0.0;

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        isDynamic = (mass != 0.f);

        localInertia = btVector3(0, 0, 0);

        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

    {
        //create a dynamic rigidbody

        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        colShape = new btSphereShape(btScalar(1.));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        startTransform.setIdentity();

        mass = 1.0;

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        isDynamic = (mass != 0.f);

        localInertia = btVector3(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(0, 4, 0));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }

}

//----------------------------------------------------
void PhysicsSimulator::simStep()
{
///-----stepsimulation_start-----

    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody *body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        { body->getMotionState()->getWorldTransform(trans); }
        else
        { trans = obj->getWorldTransform(); }
        //printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

        //JRB: Working on moving Bullet info to OpenGL to render.
        if (j == dynamicsWorld->getNumCollisionObjects() - 1)       //If sphere
        {
            ctrl->updateModelMat(1.0f, trans.getOrigin());
        }
    }
///-----stepsimulation_end-----
}

//----------------------------------------------------
void PhysicsSimulator::simClose()
{
//cleanup in the reverse order of creation/initialization
///-----cleanup_start-----

    //remove the rigidbodies from the dynamics world and delete them
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++) {
        btCollisionShape *shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    delete dynamicsWorld; //delete dynamics world
    delete solver; //delete solver
    delete overlappingPairCache; //delete broadphase
    delete dispatcher; //delete dispatcher
    delete collisionConfiguration;

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();
}