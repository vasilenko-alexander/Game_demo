//
// Created by alex on 1/14/18.
//

#pragma once

#include <string>

#define DECL_STR_CONST(name) const std::string name = #name;

namespace NSGame
{
    DECL_STR_CONST(background_music)
    DECL_STR_CONST(tank_move)
    DECL_STR_CONST(tank_shoot)
    DECL_STR_CONST(tank_explotion)

    const std::string texture_config = "config/texture_config.txt";
    const std::string level_one      = "config/level.txt";
}

#undef DECL_STR_CONST