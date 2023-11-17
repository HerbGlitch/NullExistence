#pragma once
#include <vector>

namespace roguespace {
    namespace world {
        typedef std::vector<std::vector<unsigned char>> CharMap;

        CharMap CharMap_GetFromCString(char *mapString);

        // void CharMap_Render(CharMap &map, TileMap &tiles, double scale, ARC_FPoint offset);

        void CharMap_Print(CharMap &map);
    }
}