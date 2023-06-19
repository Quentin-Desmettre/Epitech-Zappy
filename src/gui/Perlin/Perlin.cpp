#include "Perlin/Perlin.hpp"

namespace Perlin {
    float noise(int seed, int x, int y)
    {
        int n = x + y * 57 + seed * 131;

        n = (n << 13) ^ n;
        return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /
        1073741824.0);
    }

    double smoothed_noise(int i, int x, int y)
    {
        double corners = (noise(i, x - 1, y - 1) + noise(i, x + 1, y - 1) +
        noise(i, x - 1, y + 1) + noise(i, x + 1, y + 1)) / 16;
        double sides = (noise(i, x - 1, y) + noise(i, x + 1, y) + noise(i, x, y - 1)
        + noise(i, x, y + 1)) / 8;
        double center = noise(i, x, y) / 4;
        return corners + sides + center;
    }


    double interpolate(double a, double b, double x)
    {
        double ft = x * 3.1415927;
        double f = (1 - cos(ft)) * 0.5;

        return  a * (1 - f) + b * f;
    }

    double interpolated_noise(int i, double x, double y)
    {
        int int_x = static_cast<int>(x);
        double rest_x = x - int_x;
        int int_y = static_cast<int>(y);
        double rest_y = y - int_y;
        double i1 = interpolate(smoothed_noise(i, int_x, int_y)
        , smoothed_noise(i, int_x + 1, int_y), rest_x);
        double i2 = interpolate(smoothed_noise(i, int_x, int_y + 1)
        , smoothed_noise(i, int_x + 1, int_y + 1), rest_x);

        return interpolate(i1, i2, rest_y);
    }

    double Noise2D(int rng, double x, double y, int num_octaves)
    {
        double total = 0;
        double frequency = std::pow(2, num_octaves);
        double amplitude = 1;
        double persistence = 0.75;

        for (int i = 0; i < num_octaves; ++i) {
            frequency /= 2;
            amplitude *= persistence;
            total += interpolated_noise(rng + i, x / frequency, y / frequency)
            * amplitude;
        }
        return total / frequency;
    }
}