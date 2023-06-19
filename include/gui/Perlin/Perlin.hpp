
#pragma once

#include <iostream>
#include <cmath>

namespace Perlin {
    float noise(int seed, int x, int y);
    double smoothed_noise(int i, int x, int y);
    double interpolate(double a, double b, double x);
    double interpolated_noise(int i, double x, double y);
    double Noise2D(int rng, double x, double y, int num_octaves);
}
