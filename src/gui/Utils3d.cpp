#include <raylib.h>
#include <math.h>
#include "Utils3d.hpp"

void Utils::generateCirclePoints(Mateyak::Vec3f pos, Mateyak::Vec3f dir, float rayon, int nb_point, std::vector<Mateyak::Vec3f> &vec) {
    Mateyak::Vec3f up{0.0f, 1.0f, 0.0f}; 
    Mateyak::Vec3f right = dir.CrossProduct(up).Normalize();
    up = right.CrossProduct(dir).Normalize();

    const float angleIncrement = 360.0f / nb_point; 
    
    for (int i = 0; i < nb_point; ++i) {
        const float angle = angleIncrement * i * (3.14159f / 180.0f);
        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);
        
        Mateyak::Vec3f point {
            pos.x + rayon * (cosAngle * right.x + sinAngle * up.x),
            pos.y + rayon * (cosAngle * right.y + sinAngle * up.y),
            pos.z + rayon * (cosAngle * right.z + sinAngle * up.z)
        };
        
        vec.push_back(point); 
    }
}

std::vector<Mateyak::Triangle> Utils::connectPointsWithTriangles(std::vector<Mateyak::Vec3f>& points, int pointsPerRow) {
    std::vector<Mateyak::Triangle> connectedPoints;
    const int numRows = points.size() / pointsPerRow;

    for (int row = 0; row < numRows - 1; ++row) {
        for (int col = 0; col < pointsPerRow; ++col) {
            const int currentIndex = row * pointsPerRow + col;
            const int next = (col == pointsPerRow - 1) ? row * pointsPerRow : currentIndex + 1;
            const int nnext = (col == pointsPerRow - 1) ? currentIndex + 1: currentIndex + pointsPerRow + 1;

            connectedPoints.emplace_back(points[currentIndex], points[currentIndex + pointsPerRow], points[nnext], RED);
            connectedPoints.emplace_back(points[currentIndex], points[nnext], points[next], RED);
        }
    }
    return connectedPoints;
}

float Utils::differenceAngle(Mateyak::Vec3f v1, Mateyak::Vec3f v2) {
    float dotProduct = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    float angle = std::acos(dotProduct) * (180.0f / M_PI);
    return angle;
}

void Utils::drawGrid(Mateyak::Vec2f slices, float spacing, Mateyak::Vec3f pos) {
    for (int i = 0; i <= slices.x; ++i) {
        DrawLine3D(
            {pos.x + i * spacing, pos.y, pos.z},
            {pos.x + i * spacing, pos.y, pos.z + slices.y * spacing},
            WHITE
        );
    }
    for (int i = 0; i <= slices.y; ++i) {
        DrawLine3D(
        {pos.x, pos.y, pos.z + i * spacing},
        {pos.x + slices.x * spacing, pos.y, pos.z + i * spacing},
        WHITE
        );
    }
}