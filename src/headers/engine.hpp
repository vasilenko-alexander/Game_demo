#pragma once

#include "engine_export.hpp"
#include "engine_types.hpp"
#include <cstdlib>
#include <string>

namespace ge
{
    class GE_DECLSPEC IEngine
    {
    public:
        virtual ~IEngine();
        /**
         * init_options you can find in engine_constants.hpp
         */
        virtual std::string init_engine(std::string init_options) = 0;
        virtual bool read_event(event& event)                     = 0;
        virtual void uninit_engine()                              = 0;
        virtual void add_to_buffer(texture& tx)                   = 0;
        virtual void draw_buffer()                                = 0;
        virtual void swap_buffers()                               = 0;
        virtual float get_time()                                  = 0;
        virtual triangle transform_triangle(const triangle& trSrc,
                                            const triangle& trDest,
                                            float alpha) = 0;
        virtual void init_atlas(const std::string& path) = 0;
        virtual void play(const std::string& sound_name) = 0;
        virtual void stop(const std::string& sound_name) = 0;
    };

    IEngine* GE_DECLSPEC getInstance();
    std::istream& GE_DECLSPEC operator>>(std::istream& is, vertex& v);
    std::istream& GE_DECLSPEC operator>>(std::istream& is, triangle& tr);
    std::istream& GE_DECLSPEC operator>>(std::istream& is, texture& tx);
    vec3 GE_DECLSPEC operator*(const vec3& in, const matrix3& transform);
    vertex GE_DECLSPEC operator*(const vertex& in, const matrix3& transform);
    vertex GE_DECLSPEC operator*(const vertex& in, const matrix4& transform);
    matrix3 GE_DECLSPEC operator*(const matrix3& mat1, const matrix3& mat2);
    matrix4 GE_DECLSPEC operator*(const matrix4& mat1, const matrix4& mat2);
}
