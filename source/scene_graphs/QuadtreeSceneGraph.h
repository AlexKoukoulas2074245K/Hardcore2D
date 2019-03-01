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

class EntityComponentManager;

class QuadtreeSceneGraph final: public ISceneGraph
{
public:
    QuadtreeSceneGraph(const EntityComponentManager&, const glm::vec2& position, const glm::vec2& mDimensions, const int depth = 0);
    ~QuadtreeSceneGraph();
    
    std::list<EntityId> VGetCollisionCandidates(const EntityId mReferenceEntityId) override;
    void VCreate(const std::vector<EntityNameIdEntry>& mPhyicsSimulatedEntities) override;
    void VClear() override;
    
private:
    static const int MAX_OBJECTS_PER_NODE;
    static const int MAX_DEPTH;
    
    void InternalClear();
    
    const EntityComponentManager& mEntityComponentManager;
    const glm::vec2 mPosition;
    const glm::vec2 mDimensions;
    const int mDepth;
    
    QuadtreeSceneGraph* mNodes[4];
    std::list<EntityId> mObjectsInNode;
};

#endif /* QuadtreeSceneGraph_h */
