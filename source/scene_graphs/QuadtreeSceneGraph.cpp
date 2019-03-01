//
//  QuadtreeSceneGraph.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 28/02/2019.
//

#include "QuadtreeSceneGraph.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

const int QuadtreeSceneGraph::MAX_OBJECTS_PER_NODE = 10;
const int QuadtreeSceneGraph::MAX_DEPTH = 5;

QuadtreeSceneGraph::QuadtreeSceneGraph(const EntityComponentManager& entityComponentManager, const glm::vec2& position, const glm::vec2& dimensions, const int depth /* 0 */)
    : mEntityComponentManager(entityComponentManager)
    , mPosition(position)
    , mDimensions(dimensions)
    , mDepth(depth)
{
    for (int i = 0; i < 4; ++i)
    {
        mNodes[i] = nullptr;
    }
    
    InternalClear();
}

QuadtreeSceneGraph::~QuadtreeSceneGraph()
{
    InternalClear();
}

std::list<EntityId> QuadtreeSceneGraph::VGetCollisionCandidates(const EntityId)// mReferenceEntityId)
{
    std::list<EntityId> collisionCandidates;
    return collisionCandidates;
}

void QuadtreeSceneGraph::VPopulateSceneGraph(const std::vector<EntityNameIdEntry>&) //mPhyicsSimulatedEntities)
{
    const_cast<EntityComponentManager&>(mEntityComponentManager).GetComponent<PhysicsComponent>(0).GetAngularVelocity() = mDimensions.x + mPosition.x + mDepth;
}

void QuadtreeSceneGraph::VClear()
{
    InternalClear();
}

void QuadtreeSceneGraph::InternalClear()
{
    mObjectsInNode.clear();
    for (int i = 0; i < 4; ++i)
    {
        if (mNodes[i] != nullptr)
        {
            mNodes[i]->InternalClear();            
        }
        mNodes[i] = nullptr;
    }
}

void QuadtreeSceneGraph::Split()
{

}
