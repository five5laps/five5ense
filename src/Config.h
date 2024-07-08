#pragma once
#include <string>
#include <array>

// Helper struct for colors
struct RGBAColor {
    float r, g, b, a;
};

namespace Config
{
    namespace Debug {
        extern bool debug;
    }

    namespace Aimbot 
    {
        namespace Players 
        {
            extern bool enemies;
            extern bool teammates;
        };
    }

    namespace Visuals 
    {
        namespace ESP {
            namespace Players {
                extern bool enemies;
                extern bool name;
                extern bool gameClass;
                extern bool healthBar;
                extern bool teammates;
            };

            namespace Buildings {
                extern bool buildings;
                extern bool healthBar;
                extern bool buildingType;
            };

            namespace ColoredModels
            {
                extern bool player;
                extern bool playerBehindWall;
                extern bool teammates;
            };
        };

        namespace Other 
        {
            extern bool radar;
            extern int droppedWeapons;
        };

        namespace Effects 
        {
            extern bool removeFog;
            extern bool removeGrass;
        };

    };
    
    namespace Misc 
    {
        extern float overrideFov;
        extern bool bunnyHop;

        namespace Trigger {
            namespace AutoAirBlast {
                extern bool autoAirBlast;
                extern bool autoAirBlastTest;
                extern bool silent;
                extern bool rage;
            };

        };

    };
};
