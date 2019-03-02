//
//  ISceneGraph.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 28/02/2019.
//

#ifndef ISceneGraph_h
#define ISceneGraph_h

#include "../game/GameTypeTraits.h"
#include "../util/MathUtils.h"

#include <vector>
#include <list>
#include <utility>

class ISceneGraph
{
public:
    virtual ~ISceneGraph() = default;
    ISceneGraph(const ISceneGraph&) = delete;
    ISceneGraph& operator = (const ISceneGraph&) = delete;
    
    virtual std::list<EntityId> VGetCollisionCandidates(const EntityId referenceEntityId) const = 0;
    virtual void VPopulateSceneGraph(const std::vector<EntityNameIdEntry>& phyicsSimulatedEntities) = 0;    
    virtual void VClear() = 0;
    virtual std::list<std::pair<glm::vec2, glm::vec2>> VGetDebugRenderRectangles() const = 0;

protected:
    ISceneGraph() = default;
};

#endif /* ISceneGraph_h */
