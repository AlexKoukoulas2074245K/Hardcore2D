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
const int QuadtreeSceneGraph::MAX_DEPTH = 2;

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

std::list<EntityId> QuadtreeSceneGraph::VGetCollisionCandidates(const EntityId referenceEntityId)
{
    std::list<EntityId> collisionCandidates;
    const auto& entityTranslation = mEntityComponentManager.GetComponent<TransformComponent>(referenceEntityId).GetTranslation();
    const auto& entityHitbox = mEntityComponentManager.GetComponent<PhysicsComponent>(referenceEntityId).GetHitBox();
    InternalGetCollisionCandidates(referenceEntityId, entityTranslation + glm::vec3(entityHitbox.mCenterPoint.x, entityHitbox.mCenterPoint.y, 0.0f), entityHitbox.mDimensions, collisionCandidates);
    return collisionCandidates;
}

void QuadtreeSceneGraph::VPopulateSceneGraph(const std::vector<EntityNameIdEntry>& phyicsSimulatedEntities)
{
    for (const auto& entityEntry: phyicsSimulatedEntities)
    {
        if (mEntityComponentManager.HasComponent<PhysicsComponent>(entityEntry.mEntityId))
        {
            const auto& entityTranslation = mEntityComponentManager.GetComponent<TransformComponent>(entityEntry.mEntityId).GetTranslation();
            const auto& entityHitbox = mEntityComponentManager.GetComponent<PhysicsComponent>(entityEntry.mEntityId).GetHitBox();
            
            InsertObject(entityEntry.mEntityId, entityTranslation + glm::vec3(entityHitbox.mCenterPoint.x, entityHitbox.mCenterPoint.y, 0.0f), entityHitbox.mDimensions);
        }
    }
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

void QuadtreeSceneGraph::InternalGetCollisionCandidates(const EntityId referenceEntityId, const glm::vec3& objectPosition, const glm::vec2& objectDimensions, std::list<EntityId>& collisionCandidates)
{
    if (mNodes[0] != nullptr)
    {
        const auto quadrantIndex = GetMatchedQuadrant(objectPosition, objectDimensions);
        if (quadrantIndex != -1)
        {
            mNodes[quadrantIndex]->InternalGetCollisionCandidates(referenceEntityId, objectPosition, objectDimensions, collisionCandidates);
        }
    }

    auto objectsIter = mObjectsInNode.begin();
    while (objectsIter != mObjectsInNode.end())
    {
        collisionCandidates.push_back(objectsIter->mEntityId);
        objectsIter++;
    }    
}

void QuadtreeSceneGraph::Split()
{
    const auto halfWidth = mDimensions.x * 0.5f;
    const auto halfHeight = mDimensions.y * 0.5f;
    
    mNodes[0] = std::make_unique<QuadtreeSceneGraph>(mEntityComponentManager, glm::vec2(mPosition.x,             mPosition.y), glm::vec2(halfWidth, halfHeight), mDepth + 1);
    mNodes[1] = std::make_unique<QuadtreeSceneGraph>(mEntityComponentManager, glm::vec2(mPosition.x + halfWidth, mPosition.y), glm::vec2(halfWidth, halfHeight), mDepth + 1);
    mNodes[2] = std::make_unique<QuadtreeSceneGraph>(mEntityComponentManager, glm::vec2(mPosition.x + halfWidth, mPosition.y + halfHeight), glm::vec2(halfWidth, halfHeight), mDepth + 1);
    mNodes[3] = std::make_unique<QuadtreeSceneGraph>(mEntityComponentManager, glm::vec2(mPosition.x,             mPosition.y + halfHeight), glm::vec2(halfWidth, halfHeight), mDepth + 1);
}

int QuadtreeSceneGraph::GetMatchedQuadrant(const glm::vec3& objectPosition, const glm::vec2& objectDimensions)
{
    int quadrant = -1;
    
    const auto objectHalfWidth = objectDimensions.x * 0.5f;
    const auto objectHalfHeight = objectDimensions.y * 0.5f;
    const auto halfSpaceVerticalLinePoint = mPosition.x + mDimensions.x * 0.5f;
    const auto halfSpaceHorizontalLinePoint = mPosition.y + mDimensions.y * 0.5f;
    
    // completely fits in the left halfspace region of the current node
    if (objectPosition.x + objectHalfWidth < halfSpaceVerticalLinePoint)
    {
        // completely fits in the bottom left quadrant
        if (objectPosition.y + objectHalfHeight < halfSpaceHorizontalLinePoint)
        {
            quadrant = 0;
        }
        // completly fits in the top left quadrant
        else if (objectPosition.y - objectHalfHeight > halfSpaceHorizontalLinePoint)
        {
            quadrant = 3;
        }
    }
    // completely fits in the right halfspace region of the current node
    else if (objectPosition.x - objectHalfWidth > halfSpaceVerticalLinePoint)
    {
        // completely fits in the bottom right quadrant
        if (objectPosition.y + objectHalfHeight < halfSpaceHorizontalLinePoint)
        {
            quadrant = 1;
        }
        // completly fits in the top right quadrant
        else if (objectPosition.y - objectHalfHeight > halfSpaceHorizontalLinePoint)
        {
            quadrant = 2;
        }
    }
    
    return quadrant;
}

void QuadtreeSceneGraph::InsertObject(const EntityId entityId, const glm::vec3& objectPosition, const glm::vec2& objectDimensions)
{
    const auto isQuadtreeSplit = mNodes[0] != nullptr;
    
    if (isQuadtreeSplit)
    {
        const auto quadrantIndex = GetMatchedQuadrant(objectPosition, objectDimensions);
        
        if (quadrantIndex != -1)
        {
            mNodes[quadrantIndex]->InsertObject(entityId, objectPosition, objectDimensions);
            return;
        }
    }
    
    mObjectsInNode.push_back(QuadtreeEntityEntry(entityId, objectPosition, objectDimensions));
    
    if (mObjectsInNode.size() > MAX_OBJECTS_PER_NODE && mDepth < MAX_DEPTH)
    {
        if (!isQuadtreeSplit)
        {
            Split();
        }
        
        auto objectsIter = mObjectsInNode.begin();
        while (objectsIter != mObjectsInNode.end())
        {
            const auto objectQuadrantIndex = GetMatchedQuadrant(objectsIter->mObjectPosition, objectsIter->mObjectDimensions);
            if (objectQuadrantIndex != -1)
            {
                mNodes[objectQuadrantIndex]->InsertObject(objectsIter->mEntityId, objectsIter->mObjectPosition, objectsIter->mObjectDimensions);
                objectsIter = mObjectsInNode.erase(objectsIter);
            }
            else
            {
                objectsIter++;
            }
        }
    }
}
