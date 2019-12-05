#include <iostream>

#include "Queue.h"

using namespace std;

#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D{
    public:
        int x;
        int y;
        int z;

        Vector3D(int x_ = 0, int y_ = 0, int z_ = 0):
            x(x_), y(y_), z(z_){}
        
        Vector3D *GetVectorTo(Coordinate start, Coordinate end){
            x = end.x - start.x;
            y = end.y - start.y;
            z = end.z - start.z;

            return this;
        }

        bool operator>(Vector3D other){
            return x >= other.x && y >= other.y && z >= other.z;
        }

        bool operator>(Vector3D *other){
            return x >= other->x && y >= other->y && z >= other->z;
        }

        bool IsColinear(Vector3D *other){
            if(*this > other){
                if((x % other->x == 0) && (y % other->y == 0) && (z % other->z == 0)){
                    int scalar_x = x / other->x;
                    int scalar_y = y / other->y;
                    int scalar_z = z / other ->z;
                    return scalar_x == scalar_y && scalar_y==scalar_z;

                }
                else return false;
            }
            else{
                if ((other->x % x == 0) && (other->y % y == 0) && (other->z % z == 0)){
                    int scalar_x = other->x / x;
                    int scalar_y = other->y / y;
                    int scalar_z = other->z / z;
                    return scalar_x == scalar_y && scalar_y==scalar_z;
                }
                else return false;
            }
        }

};

#endif