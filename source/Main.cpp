//
//  Main.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 05/01/2019.
//

#include "App.h"

int main(int, char**)
{
    
    App app;
    
    if (app.Initialize())
    {
        app.Run();
    }
}

