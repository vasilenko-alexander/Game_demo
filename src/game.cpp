//
// Created by alex on 1/14/18.
//

#include "game.hpp"
#include "headers/engine_constants.hpp"
#include "headers/game_constants.hpp"
#include <fstream>
#include <cmath>
#include <sstream>
#include <iterator>
#include <list>

namespace NSGame
{

    ge::matrix3 Game::rotate_to(ge::keys direct)
    {
        using namespace ge;
        rotate_direction r_dir = rotate_direction::clockwise;
        float angle            = 0.f;
        switch (direct)
        {
            case keys::right:
                angle = M_PI_2;
                break;
            case keys::left:
                angle = M_PI_2;
                r_dir = rotate_direction::counterclockwise;
                break;
            case keys::down:
                angle = M_PI;
                break;
            default:
                break;
        }
        return matrix3::rotation(angle, r_dir);
    }

    ge::matrix3 Game::move_to(ge::keys direct)
    {
        using namespace ge;
        static float dx = 0.f;
        static float dy = 0.f;
        switch (direct)
        {
            case keys::up:
                dy += 0.01f;
                break;
            case keys::down:
                dy -= 0.01f;
                break;
            case keys::left:
                dx -= 0.01f;
                break;
            case keys::right:
                dx += 0.01f;
                break;
            default:
                break;
        }
        return matrix3::transport(dx, dy);
    }

    Game::Game()
    {
    }

    Game::~Game()
    {
        if (battle_ground != nullptr)
            delete battle_ground;

        if (!obj_textures_coords.empty())
        {
            for (auto it = obj_textures_coords.begin();
                 it != obj_textures_coords.end();
                 ++it)
            {
                delete it->second;
            }
        }

        if (!game_objs.empty())
        {
            for (size_t i = 0; i < game_objs.size(); ++i)
                delete game_objs[i];
        }

        if (engine)
            engine->uninit_engine();
    }

    void Game::init()
    {
        engine = ge::getInstance();
    }

    ge::matrix4 Game::get_norm_mat()
    {
        ge::matrix4 norm_mat = ge::matrix4::indentity();
        float norm_coeff     = battle_ground_size / 2.f;
        float shift_coeff    = 1.f;

        norm_mat.data[0].x /= norm_coeff;
        norm_mat.data[1].y /= norm_coeff;
        norm_mat.data[3].x -= shift_coeff;
        norm_mat.data[3].y -= shift_coeff;

        return norm_mat;
    }

    bool Game::init_level(const std::string& level_path)
    {
        std::ifstream file(level_path);
        if (!file.is_open())
        {
            std::cerr << "Can't open file: " << level_path << std::endl;
            return false;
        }

        std::list<std::string> lvl_lst;
        for (std::string line_objs; std::getline(file, line_objs);)
        {
            lvl_lst.push_front(line_objs);
        }
        int y_shift = 0;
        for (auto it = lvl_lst.begin(); it != lvl_lst.end(); ++it, ++y_shift)
        {
            std::istringstream line_parser(it->c_str());
            int x_shift = 0;
            for (std::string obj_type_str;
                 std::getline(line_parser, obj_type_str, '|');
                 ++x_shift)
            {
                ObjTypes type =
                    static_cast<ObjTypes>(atoi(obj_type_str.c_str()));
                if (Undefined != type && Ground != type)
                {
                    GameObject* g_obj = get_game_obj(type, x_shift, y_shift);
                    game_objs.push_back(g_obj);
                }
            }
        }
        return !game_objs.empty();
    }

    GameObject* Game::get_game_obj(ObjTypes obj_type, int x_shift, int y_shift)
    {
        float coeff = 0.1f;
        float size  = 0.f;

        switch (obj_type)
        {
            case PlayerTank:
                break;
            case Wall:
                break;
            case Ground:
                coeff = 1.f;
                break;
            case Bullet:
                coeff = 0.01f;
                break;
            case Undefined:
            default:
                obj_type = Undefined;
                break;
        }
        size = battle_ground_size * coeff;

        GameObject* game_obj = new GameObject();
        game_obj->type       = obj_type;
        game_obj->rect       = ObjRect::get_rect(size, x_shift, y_shift);
        normalizing(game_obj->rect);

        return game_obj;
    }

    void Game::init_atlas(std::string config_path)
    {
        std::ifstream texture_file(config_path);
        if (!texture_file.is_open())
        {
            std::cerr << "Can't open config file: " << config_path << std::endl;
            return;
        }
        std::string atlas_path;

        float x_step = 0.f;
        float y_step = 0.f;
        for (std::string line; std::getline(texture_file, line);)
        {
            std::istringstream line_parse(line);
            int type    = 0;
            int x_shift = 0;
            int y_shift = 0;
            if (atlas_path.empty())
            {
                int x_count = 0;
                int y_count = 0;
                line_parse >> atlas_path >> x_count >> y_count;
                if (line_parse.bad())
                {
                    std::cerr << "Atlas path reading failed" << std::endl;
                    break;
                }
                x_step = 1.f / x_count;
                y_step = 1.f / y_count;
                engine->init_atlas(atlas_path);
            }
            else
            {
                line_parse >> type >> x_shift >> y_shift;
                if (line_parse.bad())
                {
                    std::cerr << "Obj data reading failed" << std::endl;
                    continue;
                }
                ObjTypes key_type  = static_cast<ObjTypes>(type);
                ObjRect* type_rect = new ObjRect(
                    ObjRect::get_rect(x_step, y_step, x_shift, y_shift));
                obj_textures_coords[key_type] = type_rect;
            }
        }
    }

    void Game::normalizing(ObjRect& rect)
    {
        ge::matrix4 norm_mat = get_norm_mat();
        for (size_t i = 0; i < rect.left.v.size(); ++i)
        {
            rect.left.v[i]  = rect.left.v[i] * norm_mat;
            rect.right.v[i] = rect.right.v[i] * norm_mat;
        }
    }

    void Game::draw_all_objs()
    {
        ge::texture text;
        for (const GameObject* obj : game_objs)
        {
            text.coords     = obj->rect.left.v;
            text.tex_coords = obj_textures_coords.at(obj->type)->left.v;
            engine->add_to_buffer(text);
            text.coords     = obj->rect.right.v;
            text.tex_coords = obj_textures_coords.at(obj->type)->right.v;
            engine->add_to_buffer(text);
        }
        engine->draw_buffer();
        engine->swap_buffers();
    }

    int Game::exec()
    {
        init();
        std::string errMsg = engine->init_engine(ge::everything);
        if (!errMsg.empty())
        {
            std::cerr << errMsg << std::endl;
            return EXIT_FAILURE;
        }

        init_atlas(texture_config);
        battle_ground = get_game_obj(Ground, 0, 0);
        if (!init_level(level_one))
            return EXIT_FAILURE;

        const ge::matrix4& norm_mat = get_norm_mat();

        bool run_loop = true;
        ge::event event;
        ge::matrix3 move   = ge::matrix3::indentity();
        ge::matrix3 rotate = ge::matrix3::indentity();
        //        float last_time    = engine->get_time();
        //        uint frames_count  = 0;
        engine->play(NSGame::background_music);
        while (run_loop)
        {
            while (engine->read_event(event))
            {
                switch (event.type)
                {
                    case ge::events_t::shutdown:
                        run_loop = false;
                        break;
                    case ge::events_t::pressed:
                        move   = move_to(event.key);
                        rotate = rotate_to(event.key);
                        if (event.key == ge::keys::button1)
                            engine->play(NSGame::tank_explotion);
                        if (event.key == ge::keys::button2)
                            engine->play(NSGame::tank_shoot);
                        break;
                    default:
                        break;
                }
            }
            //            const std::string vertexes_path =
            //            "config/texture_vertexes.txt";
            //
            //            std::ifstream file(vertexes_path);
            //            if (!file.is_open())
            //            {
            //                std::cerr << "Can't open file with vertexes
            //                values"
            //                          << std::endl;
            //                continue;
            //            }

            //            ge::texture txLeft, txRight;
            //            file >> txLeft >> txRight;
            //
            //            ge::matrix3 transform = rotate * move;
            //            ge::matrix4 trans_mat(transform);
            //
            //            trans_mat = norm_mat * trans_mat;
            //
            //            for (size_t v_i = 0, max_i = txRight.coords.size();
            //            v_i < max_i;
            //                 ++v_i)
            //            {
            //                txRight.coords[v_i] = txRight.coords[v_i] *
            //                trans_mat;
            //                txLeft.coords[v_i]  = txLeft.coords[v_i] *
            //                trans_mat;
            //            }

            //            ge::texture test_one;
            //            test_one.coords.clear();
            //            test_one.tex_coords.clear();
            //            test_one.coords.insert(test_one.coords.end(),
            //                                   txLeft.coords.begin(),
            //                                   txLeft.coords.end());
            //            test_one.coords.insert(test_one.coords.end(),
            //                                   txRight.coords.begin(),
            //                                   txRight.coords.end());
            //            test_one.tex_coords.insert(test_one.tex_coords.end(),
            //                                       txLeft.tex_coords.begin(),
            //                                       txLeft.tex_coords.end());
            //            test_one.tex_coords.insert(test_one.tex_coords.end(),
            //                                       txRight.tex_coords.begin(),
            //                                       txRight.tex_coords.end());

            draw_all_objs();

            //            ++frames_count;
            //            float new_time  = engine->get_time();
            //            float time_diff = new_time - last_time;
            //            if (time_diff >= 1.f)
            //            {
            //                std::cout << frames_count << " " << time_diff <<
            //                std::endl;
            //                last_time    = new_time;
            //                frames_count = 0;
            //            }
        }
        engine->stop(NSGame::background_music);
        return EXIT_SUCCESS;
    }
}