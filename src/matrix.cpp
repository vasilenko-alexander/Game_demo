#include "headers/engine_types.hpp"
#include <cmath>

namespace ge
{
    matrix3::matrix3()
    {
    }
    matrix3::matrix3(const std::vector<vec3>& init_vec) : data(init_vec)
    {
    }
    matrix3::matrix3(const matrix3& mat) : data(mat.data)
    {
    }

    matrix3 matrix3::indentity()
    {
        std::vector<vec3> init_vec = { vec3(1.f, 0.f, 0.f),
                                       vec3(0.f, 1.f, 0.f),
                                       vec3(0.f, 0.f, 1.f) };
        return matrix3(init_vec);
    }

    matrix3 matrix3::scale(float scale)
    {
        matrix3 scaling   = indentity();
        scaling.data[0].x = scale;
        scaling.data[1].y = scale;
        return scaling;
    }

    matrix3 matrix3::rotation(float thetha, rotate_direction direct)
    {
        matrix3 rotation   = matrix3::indentity();
        rotation.data[0].x = std::cos(thetha);
        rotation.data[1].y = std::cos(thetha);
        if (direct == rotate_direction::clockwise)
        {
            rotation.data[0].y = -std::sin(thetha);
            rotation.data[1].x = std::sin(thetha);
        }
        else
        {
            rotation.data[0].y = std::sin(thetha);
            rotation.data[1].x = -std::sin(thetha);
        }
        return rotation;
    }

    matrix3 matrix3::transport(float dx, float dy)
    {
        matrix3 transport   = matrix3::indentity();
        transport.data[2].x = dx;
        transport.data[2].y = dy;
        return transport;
    }

    void matrix3::operator=(const matrix3& mat)
    {
        this->data = mat.data;
    }

    std::vector<float> matrix3::to_array() const
    {
        std::vector<float> mat_array;
        for (size_t i = 0; i < data.size(); ++i)
        {
            mat_array.push_back(data[i].x);
            mat_array.push_back(data[i].y);
            mat_array.push_back(data[i].w);
        }

        return mat_array;
    }

    matrix4::matrix4()
    {
    }

    matrix4::matrix4(const std::vector<vec4>& init_vec) : data(init_vec)
    {
    }

    matrix4::matrix4(const matrix4& init_mat) : data(init_mat.data)
    {
    }

    matrix4::matrix4(const matrix3& init_mat)
    {
        data[0] = vec4(init_mat.data[0]);
        data[1] = vec4(init_mat.data[1]);
        data[2] = vec4(0.f, 0.f, 1.f, 0.f);
        data[3] = vec4(init_mat.data[2]);
    }

    void matrix4::operator=(const matrix4& mat)
    {
        this->data = mat.data;
    }

    matrix4 matrix4::indentity()
    {
        std::vector<vec4> init_vec = { vec4(1.f, 0.f, 0.f, 0.f),
                                       vec4(0.f, 1.f, 0.f, 0.f),
                                       vec4(0.f, 0.f, 1.f, 0.f),
                                       vec4(0.f, 0.f, 0.f, 1.f) };
        return matrix4(init_vec);
    }

    std::vector<float> matrix4::to_array() const
    {
        std::vector<float> mat_array;
        for (size_t i = 0; i < data.size(); ++i)
        {
            mat_array.push_back(data[i].x);
            mat_array.push_back(data[i].y);
            mat_array.push_back(data[i].z);
            mat_array.push_back(data[i].w);
        }

        return mat_array;
    }
}
