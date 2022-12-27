#pragma once

#include <cmath>

struct IdPair
{
    int vertexId0;
    int vertexId1;
};

struct Double3
{
    double x, y, z;

    static double Distance(Double3 left, Double3 right);
};

inline
double Double3::Distance(Double3 left, Double3 right)
{
    //TODO:
    double x = left.x - right.x;
    double y = left.y - right.y;
    double z = left.z - right.z;
    return sqrt(x*x + y*y + z*z);
}