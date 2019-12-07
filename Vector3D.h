/**********************************************************
    Vector3D.h      Author: Eduardo Nodarse
                    10/27/2019
 ==========================================================
 -  This header is used to construct 3D vectors and test
    for colinear...arity
**********************************************************/
#include <iostream>

#include "Queue.h"

using namespace std;

#ifndef VECTOR3D_H
#define VECTOR3D_H

//Class that keeps track of direction of the virector
class Vector3D{
    public:
        //x, y and z components of the vector
        int x;
        int y;
        int z;

        //Constructor with default values
        Vector3D(int x_ = 0, int y_ = 0, int z_ = 0):
            x(x_), y(y_), z(z_){}
        
        //Returns this vector between two points
        Vector3D *GetVectorTo(Coordinate start, Coordinate end){
            //cout<<"Getting Vector from "<< start << " to "<< end << endl;
            x = end.x - start.x;
            y = end.y - start.y;
            z = end.z - start.z;

            return this;
        }

        //Simple comparators
        bool operator>(Vector3D other){
            return x >= other.x && y >= other.y && z >= other.z;
        }

        bool operator>(Vector3D *other){
            return x >= other->x && y >= other->y && z >= other->z;
        }

        bool operator==(Vector3D *other){
            return (x == other->x && y == other->y && z == other->z);
        }

        //Prints out the vector
        friend ostream &operator<<(ostream &out, const Vector3D &v){
            out << "<" << v.x << ", " << v.y << ", " << v.z << ">";
            return out;
        }

        /*
            A vector is colinear if the two vectors are parallel 
            to each other and if they are scalar multiples of each
            other. The two vectors will share a common point,
            and therefore colinear...ity will be determined if 
            there is some scalar multiple common amongst all the 
            components of both vectors 
        */
        bool IsColinear(Vector3D *other){
            //Zero vector used for comparison
            Vector3D zero = Vector3D();

            //Component scalar multiples
            float x_scalar;
            float y_scalar;
            float z_scalar;

            //Used for printing weather or not they are colinear
            bool ret;

            //cout<<"This: " << *this<< "\t" <<"other: " << *other << endl;

            //If the vector 0, then it is automatically colinear
            if(other == &zero || *this==&zero){
                return true;
            }

            /*
                If two of the components of one of the vectors is 
                0, then they are automatically scalar multiples
            */
            if((x == 0 && y == 0) || (x == 0 && z == 0) || (y == 0 && z == 0)){
                return true;
            }
            else if((other->x == 0 && other->y == 0) || (other->x == 0 && other->z == 0) || (other->y == 0 && other->z == 0)){
                return true;
            }
            
            /*
                If one of the components on one vector is 0, and a 
                different component on the other vector is 0, then
                they are scalar multiples
            */
            else if((x == 0 && other->y == 0) || (x == 0 && other->z == 0)){
                return false;
            }
            else if((y == 0 && other->x == 0) || (y == 0 && other->z == 0)){
                return false;
            }
            else if((z == 0 && other->x == 0) || (z == 0 && other->y == 0)){
                return false;
            }

            /*
                Checks to see if any of the components are 0, to
                avoid dividing by 0
            */
            else if(x == 0 || other->x == 0){
                //cout<<"\t\tx = 0 || other->x = 0" << endl;
                //Divides larger vector by smaller to get the value
                if(*this > other){
                    y_scalar = y / other->y;
                    z_scalar = z / other->z;

                }
                else{
                    y_scalar = other->y / y;
                    z_scalar = other->z / z;
                }
                ret = y_scalar == z_scalar;
            }
            else if(y == 0 || other->y == 0){
                //cout<<"\t\ty = 0 || other->y = 0"<<endl;
                if(*this > other){
                    x_scalar = x / other->x;
                    z_scalar = z / other->z;

                }
                else{
                    x_scalar = other->x / x;
                    z_scalar = other->z / z;
                }
                
                ret = x_scalar == z_scalar;
            }
            else if(z == 0 || other->z == 0){
                //cout<<"\t\tz = 0 || other->z = 0"<<endl;
                if(*this > other){
                    y_scalar = y / other->y;
                    x_scalar = x / other->x;

                }
                else{
                    y_scalar = other->y / y;
                    x_scalar = other->x / x;
                }
                
                ret =  y_scalar == x_scalar;
            }

            //There are no zeros, divide bigger vector
            else{
                //cout<<"\t\tNo zeros"<<endl;
                if(*this > other){
                    x_scalar = x / other->x;
                    y_scalar = y / other->y;
                    z_scalar = z / other->z;

                }
                else{
                    x_scalar = other->x / x;
                    y_scalar = other->y / y;
                    z_scalar = other->z / z;
                }
                
                ret = x_scalar == y_scalar && y_scalar == z_scalar;
            }

            // //Debugging
            // cout<<"\t These are ";
            // if(ret)
            //     cout<<"colinear"<<endl;
            // else
            //     cout<<"not colinear"<< endl;

            return ret;
        }

        void ConvertToStep(){
            if(x != 0)
                x /= abs(x);
            
            if(y != 0)
                y /= abs(y);
            
            if(z != 0)
                z /= abs(z);
        }
};

#endif