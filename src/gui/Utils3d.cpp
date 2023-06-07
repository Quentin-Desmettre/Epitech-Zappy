#include <raylib.h>
#include <math.h>
#include "Utils3d.hpp"

Vector3 rotate(Vector3 center, Vector3 rt, float angle)
{
    float p_angle = angle * (PI / 180);
    float s = sin(p_angle);
    float c = cos(p_angle);
    float xnew;
    float ynew;

    rt.x -= center.x;
    rt.z -= center.z;
    xnew = rt.x * c - rt.z * s;
    ynew = rt.x * s + rt.z * c;
    rt.x = xnew + center.x;
    rt.z = ynew + center.z;
    return rt;
}

float distance(Vector3 pos1, Vector3 pos2)
{
    return (abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y) + abs(pos1.z - pos2.z)) / 3.0;
}

float distance_2d(Vector3 pos1, Vector3 pos2)
{
    return (abs(pos1.x - pos2.x) + abs(pos1.z - pos2.z)) / 2.0;
}

void Utils::generateCirclePoints(Mateyak::Vec3f pos, Mateyak::Vec3f dir, float rayon, int nb_point, std::vector<Mateyak::Vec3f> &vec) {
    Mateyak::Vec3f up{0.0f, 1.0f, 0.0f}; 
    Mateyak::Vec3f right = dir.CrossProduct(up);
    right = right.Normalize(); 
    up = right.CrossProduct(dir);
    up = up.Normalize(); 
    
    const float angleIncrement = 360.0f / nb_point; 
    
    for (int i = 0; i < nb_point; ++i) {
        float angle = angleIncrement * i; 
        angle = angle * (3.14159f / 180.0f);
        
        Mateyak::Vec3f point {
            pos.x + rayon * (cos(angle) * right.x + sin(angle) * up.x),
            pos.y + rayon * (cos(angle) * right.y + sin(angle) * up.y),
            pos.z + rayon * (cos(angle) * right.z + sin(angle) * up.z)
        };
        
        vec.push_back(point); 
    }
}

std::vector<Mateyak::Triangle> Utils::connectPointsWithTriangles(std::vector<Mateyak::Vec3f>& points, int pointsPerRow) {
    std::vector<Mateyak::Triangle> connectedPoints;
    const int numRows = points.size() / pointsPerRow;

    for (int row = 0; row < numRows - 1; ++row) {
        for (int col = 0; col < pointsPerRow; ++col) {
            int currentIndex = row * pointsPerRow + col;
            int next = (col == pointsPerRow - 1) ? row * pointsPerRow : currentIndex + 1;
            int nnext = (col == pointsPerRow - 1) ? currentIndex + 1: currentIndex + pointsPerRow + 1;

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

//bool isInFOV(Mateyak::Vec3f camPos, Mateyak::Vec3f camTar, float fov, Mateyak::Vec3f point)
//{
//    Mateyak::Vec3f camToPos = (point - camPos).Normalize();
//    Mateyak::Vec3f camToTar = (camTar - camPos).Normalize();
//    float dotProduct = camToPos.x * camToTar.x + camToPos.y * camToTar.y + camToPos.z * camToTar.z;
//    float angle = std::acos(dotProduct) * (180.0f / M_PI);
//
//    if (angle <= fov / 2.0f) {
//        return true;
//    } else {
//        return false;
//    }
//}