//
//  IService.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef IService_h
#define IService_h

class IService
{
public:
    virtual ~IService() = default;
    
protected:
    IService() = default;
    
private:
    IService(const IService&) = delete;
    const IService& operator=(const IService&) = delete;
};

#endif /* IService_h */
