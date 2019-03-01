//
//  ISceneGraph.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 28/02/2019.
//

#ifndef ISceneGraph_h
#define ISceneGraph_h

#include "../game/GameTypeTraits.h"

#include <vector>
#include <list>

class ISceneGraph
{
public:
    virtual ~ISceneGraph() = default;
    ISceneGraph(const ISceneGraph&) = delete;
    ISceneGraph& operator = (const ISceneGraph&) = delete;
    
    virtual void VGetCollisionCandidates(const EntityId referenceEntityId, std::list<EntityId>& collisionCandidates) = 0;
    virtual void VPopulateSceneGraph(const std::vector<EntityNameIdEntry>& phyicsSimulatedEntities) = 0;
    virtual void VClear() = 0;

protected:
    ISceneGraph() = default;
};

#endif /* ISceneGraph_h */
