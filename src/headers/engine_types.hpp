#pragma once

#include "engine_export.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace ge
{
    enum class keys
    {
        nobutton,
        up,
        down,
        left,
        right,
        pause,
        select,
        button1,
        button2
    };

    enum class events_t
    {
        noevent,
        pressed,
        released,
        shutdown
    };

    enum class rotate_direction
    {
        clockwise,
        counterclockwise
    };

    struct event
    {
        std::string msg;
        events_t type = events_t::noevent;
        keys key      = keys::nobutton;
    };

    struct GE_DECLSPEC vertex
    {
        float x = 0.f;
        float y = 0.f;

        vertex()
        {
        }
        vertex(const vertex& v) : x(v.x), y(v.y)
        {
        }

        void operator=(const vertex& v)
        {
            x = v.x;
            y = v.y;
        }
    };

    struct GE_DECLSPEC vec3
    {
        vec3()
        {
        }
        vec3(float _x, float _y, float _w) : x(_x), y(_y), w(_w)
        {
        }
        float x = 0.f;
        float y = 0.f;
        float w = 0.f;
    };

    struct GE_DECLSPEC vec4
    {
        vec4()
        {
        }

        vec4(const vec3& init_vec3)
            : x(init_vec3.x), y(init_vec3.y), w(init_vec3.w)
        {
        }

        vec4(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w)
        {
        }
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
        float w = 0.f;
    };

    struct GE_DECLSPEC matrix3
    {
        matrix3();
        matrix3(const std::vector<vec3>& init_vec);
        matrix3(const matrix3& mat);

        static matrix3 indentity();
        static matrix3 scale(float scale);
        static matrix3
        rotation(float thetha,
                 rotate_direction direct = rotate_direction::clockwise);
        static matrix3 transport(float dx, float dy);
        std::vector<float> to_array() const;
        void operator=(const matrix3& mat);

        std::vector<vec3> data = { vec3(), vec3(), vec3() };
    };

    struct GE_DECLSPEC matrix4
    {
        matrix4();
        matrix4(const std::vector<vec4>& init_vec);
        matrix4(const matrix4& init_mat);
        matrix4(const matrix3& init_mat);

        static matrix4 indentity();
        std::vector<float> to_array() const;

        void operator=(const matrix4& mat);

        std::vector<vec4> data = { vec4(), vec4(), vec4(), vec4() };
    };

    struct GE_DECLSPEC triangle
    {
        triangle()
        {
        }
        triangle(const triangle& copy_obj) : v(copy_obj.v)
        {
        }

        void operator=(const triangle& obj)
        {
            v = obj.v;
        }

        std::vector<vertex> v = { vertex(), vertex(), vertex() };
    };

    struct GE_DECLSPEC texture
    {
        std::vector<vertex> coords     = { vertex(), vertex(), vertex() };
        std::vector<vertex> tex_coords = { vertex(), vertex(), vertex() };
    };
}
