/**********************************************************
    Shapes.h            Author: Eduardo Nodarse
                        10/27/2019
 ==========================================================
 -  This header is used to construct shapes as obstacles
    for the map. The shapes are capable of detecting 
    collisions
**********************************************************/

#include <iostream>
#include "Vector3D.h"
#include "Queue.h"

using namespace std;

#ifndef SHAPES_H
#define SHAPES_H

//Interface for Spheres and Cubes
class Shapes{
    public:
        int x_center;
        int y_center;
        int z_center;

        Shapes(int x, int y, int z){
            x_center = x;
            y_center = y;
            z_center = z;
        }

        //Pure abstract function
        virtual bool DetectCollision(int x, int y, int z) = 0;
};

class Cube: public Shapes{
    public:
        int sideLength;

        Cube(int x, int y, int z, int sides): Shapes(x, y, z){
            sideLength = sides;
        } 

        //Checks to see if a point is in or on the cube
        virtual bool DetectCollision(int x, int y, int z){
            int dist = sideLength/2;

            if(x_center - dist <= x && x <= x_center + dist){
                if(y_center - dist <= y && y <= y_center + dist){
                    if(z_center - dist <= z && z <= z_center + dist){
                        //cout<<"true"<<endl;
                        return true;
                    }
                }
            }

            return false;
        }

        virtual bool DetectCollision(Vector3D thru, Coordinate start){
            Vector3D *check = new Vector3D();
            Coordinate temp;

            int dist = sideLength/2;

            for(int i = x_center - dist; i < x_center + dist; ++i){
                for (int j = y_center - dist; j < y_center + dist; ++j){
                    for(int k = z_center - dist; k < z_center +dist; ++k){
                        temp.Change(i, j, k);
                        check->GetVectorTo(start, temp);
                        if(thru.IsColinear(check)){
                            free(check);
                            check = NULL;
                            return true;
                        }
                    }
                }
            }

            free(check);
            check = NULL;
            return false;
        }
    
};

class Sphere: public Shapes{
    public:
        int radius;

        Sphere(int x, int y, int z, int r): Shapes(x, y, z) {
            radius = r;
            //cout<<"Sphere constructed, radius = " << radius << endl;
        }

        virtual bool DetectCollision(int x, int y, int z){
            int x_d = x_center - x;
            int y_d = y_center - y;
            int z_d = z_center - z;

            x_d *= x_d;
            y_d *= y_d;
            z_d *= z_d;

            int sum = z_d + x_d + y_d;

            if(sum <= (radius * radius)){
                return true;
            }
            
            return false;
        }

        virtual bool DetectCollision(Vector3D thru, Coordinate start){
            Vector3D *check = new Vector3D();
            Coordinate temp;

            for(int i = x_center - radius; i < x_center + radius; ++i){
                for (int j = y_center - radius; j < y_center + radius; ++j){
                    for(int k = z_center - radius; k < z_center +radius; ++k){
                        temp.Change(i, j, k);
                        check->GetVectorTo(start, temp);
                        if(DetectCollision(i,j,k) && thru.IsColinear(check)){
                            free(check);
                            check = NULL;
                            return true;
                        }
                    }
                }
            }
            free (check);
            check = NULL;
            return false;
        }
};
#endif