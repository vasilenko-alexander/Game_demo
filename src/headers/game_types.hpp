//
// Created by alex on 1/27/18.
//
#pragma once

#include "engine_types.hpp"
#include <vector>

namespace NSGame
{

    enum ObjTypes
    {
        Undefined = -1,
        Ground,
        PlayerTank,
        Wall,
        Bullet
    };

    struct ObjRect
    {
        ge::triangle left;
        ge::triangle right;

        ObjRect()
        {
        }
        ObjRect(const ObjRect& copy_obj)
            : left(copy_obj.left), right(copy_obj.right)
        {
        }

        void operator=(const ObjRect& obj)
        {
            left  = obj.left;
            right = obj.right;
        }

        static ObjRect get_rect(float step, int x_shift, int y_shift)
        {
            ObjRect rect;
            ge::vertex lt_corner;
            ge::vertex rt_corner;
            ge::vertex lb_corner;
            ge::vertex rb_corner;

            lb_corner.x = step * x_shift;
            lb_corner.y = step * y_shift;

            rt_corner.x = lb_corner.x + step;
            rt_corner.y = lb_corner.y + step;

            lt_corner.x = lb_corner.x;
            lt_corner.y = lb_corner.y + step;

            rb_corner.x = lb_corner.x + step;
            rb_corner.y = lb_corner.y;

            rect.left.v[0] = lt_corner;
            rect.left.v[1] = rb_corner;
            rect.left.v[2] = lb_corner;

            rect.right.v[0] = lt_corner;
            rect.right.v[1] = rt_corner;
            rect.right.v[2] = rb_corner;

            return rect;
        }
        static ObjRect
        get_rect(float x_step, float y_step, int x_shift, int y_shift)
        {
            ObjRect rect;
            ge::vertex lt_corner;
            ge::vertex rt_corner;
            ge::vertex lb_corner;
            ge::vertex rb_corner;

            lb_corner.x = x_step * x_shift;
            lb_corner.y = y_step * y_shift;

            rt_corner.x = lb_corner.x + x_step;
            rt_corner.y = lb_corner.y + y_step;

            lt_corner.x = lb_corner.x;
            lt_corner.y = lb_corner.y + y_step;

            rb_corner.x = lb_corner.x + x_step;
            rb_corner.y = lb_corner.y;

            rect.left.v[0] = lt_corner;
            rect.left.v[1] = rb_corner;
            rect.left.v[2] = lb_corner;

            rect.right.v[0] = lt_corner;
            rect.right.v[1] = rt_corner;
            rect.right.v[2] = rb_corner;

            return rect;
        }
    };

    struct GameObject
    {
        GameObject()
        {
        }
        GameObject(const GameObject& copy_obj)
            : rect(copy_obj.rect), type(copy_obj.type)
        {
        }

        void operator=(const GameObject& obj)
        {
            type = obj.type;
            rect = obj.rect;
        }

        ObjRect rect;
        ObjTypes type = Undefined;
    };
}
