//
//  LevelFactory.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#ifndef LevelFactory_h
#define LevelFactory_h

#include <memory>
#include <string>

class Level;
class ServiceLocator;

class LevelFactory final
{
public:
    LevelFactory(const ServiceLocator&);
    
    std::unique_ptr<Level> CreateLevel(const std::string& levelPath);
    
private:
    const ServiceLocator& mServiceLocator;
};

#endif /* LevelFactory_h */
