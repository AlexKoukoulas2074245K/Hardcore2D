//
//  ICommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef ICommand_h
#define ICommand_h

#include "../util/StringId.h"

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;
	virtual StringId GetCommandClassId() const = 0;
};

#endif /* ICommand_h */
