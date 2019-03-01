//
//  QuadtreeSceneGraph.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 28/02/2019.
//

#ifndef QuadtreeSceneGraph_h
#define QuadtreeSceneGraph_h

#include "ISceneGraph.h"
#include "../util/MathUtils.h"

#include <memory>

class EntityComponentManager;

class QuadtreeSceneGraph final: public ISceneGraph
{
public:
    QuadtreeSceneGraph(const EntityComponentManager&, const glm::vec2& position, const glm::vec2& mDimensions, const int depth = 0);
    ~QuadtreeSceneGraph();
    
    std::list<EntityId> VGetCollisionCandidates(const EntityId referenceEntityId) override;
    void VPopulateSceneGraph(const std::vector<EntityNameIdEntry>& phyicsSimulatedEntities) override;    
    void VClear() override;
    
private:
    static const int MAX_OBJECTS_PER_NODE;
    static const int MAX_DEPTH;
    
    struct QuadtreeEntityEntry
    {
        QuadtreeEntityEntry(const EntityId entityId, const glm::vec3& objectPosition, const glm::vec2& objectDimensions)
            : mEntityId(entityId)
            , mObjectPosition(objectPosition)
            , mObjectDimensions(objectDimensions)
        {
        }
        
        const EntityId mEntityId;
        const glm::vec3 mObjectPosition;
        const glm::vec2 mObjectDimensions;
    };
    
    void InternalClear();
    void InternalGetCollisionCandidates(const EntityId referenceEntityId, const glm::vec3& objectPosition, const glm::vec2& objectDimensions, std::list<EntityId>& collisionCandidates);
    void Split();
    int GetMatchedQuadrant(const glm::vec3& objectPosition, const glm::vec2& objectDimensions);
    void InsertObject(const EntityId entityId, const glm::vec3& objectPosition, const glm::vec2& objectDimensions);
    
    const EntityComponentManager& mEntityComponentManager;
    const glm::vec2 mPosition;
    const glm::vec2 mDimensions;
    const int mDepth;
    
    std::unique_ptr<QuadtreeSceneGraph> mNodes[4];
    std::list<QuadtreeEntityEntry> mObjectsInNode;
};

#endif /* QuadtreeSceneGraph_h */
