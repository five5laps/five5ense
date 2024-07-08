#include "Config.h"

namespace Config
{
    namespace Debug {
        bool debug = false;
    }

    namespace Aimbot
    {
        namespace Players
        {
            bool enemies = false;
            bool teammates = false;
        }
    }

    namespace Visuals
    {
        namespace ESP {
            namespace Players {
                bool enemies = false;
                bool name = false;
                bool gameClass = false;
                bool healthBar = false;
                bool teammates = false;
            };

            namespace Buildings {
                bool buildings = false;
                bool healthBar = false;
                bool buildingType = false;
            };

            namespace ColoredModels
            {
                bool player = false;
                bool playerBehindWall = false;
                bool teammates = false;
            };
        };

        namespace Other
        {
            bool radar = false;
            int droppedWeapons = 1;
        };

        namespace Effects
        {
            bool removeFog = false;
            bool removeGrass = false;
        };
    };

    namespace Misc
    {
        float overrideFov = 90.f;
        bool bunnyHop = false;

        namespace Trigger {
            namespace AutoAirBlast {
                bool autoAirBlast = false;
                bool autoAirBlastTest = false;
                bool silent = false;
                bool rage = false;
            };

        };
    };
};