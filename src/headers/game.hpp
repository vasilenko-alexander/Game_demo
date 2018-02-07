//
// Created by alex on 1/14/18.
//

#pragma once

#include "engine.hpp"
#include "game_types.hpp"
#include <map>

namespace NSGame
{
    class Game
    {
        ge::IEngine* engine;
        float battle_ground_size  = 1000.0;
        GameObject* battle_ground = nullptr;
        std::vector<GameObject*> game_objs;
        std::map<ObjTypes, ObjRect*> obj_textures_coords;

    public:
        Game();
        ~Game();
        int exec();

    private:
        void init();
        bool init_level(const std::string& level_path);
        void init_atlas(std::string config_path);
        void normalizing(ObjRect& rect);
        void draw_all_objs();
        GameObject* get_game_obj(ObjTypes obj_type, int x_shift, int y_shift);
        ge::matrix4 get_norm_mat();
        ge::matrix3 rotate_to(ge::keys direct);
        ge::matrix3 move_to(ge::keys direct);
    };
}
