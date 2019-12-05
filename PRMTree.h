#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Vector3D.h"
#include "Shapes.h"

using namespace std;

#ifndef PRMTREE_H
#define PRMTREE_H
struct TNode{
    Coordinate c;
    
    enum status{
        none,
        setup,
        visited,
        explored
    };

    status state;

    vector<TNode*> locations;

    TNode(Coordinate o){
        c = o;
        state = status::none;
    }

    void AddLocation(TNode* loc){
        if(std::find(locations.begin(), locations.end(), loc) == locations.end()){
            locations.push_back(loc);
        }
        else{
            cout<<"Element found"<<endl;
        }
    }

    float Distance(TNode *a){
        int x_d = c.x - a->c.x;
        int y_d = c.y - a->c.y;
        int z_d = c.z - a->c.z;

        x_d *= x_d;
        y_d *= y_d;
        z_d *= z_d;

        return sqrt(x_d + y_d + z_d);
    }

};

class PRMTree{
    public:
        int max_X;
        int max_Y;
        int max_Z;

        int MAX_DIST = 10;
        int MAX_CONNECTION = 3;

        vector<Shapes*> obstacles;
        vector<TNode*> locations;

        PRMTree(int x, int y, int z, vector<Shapes*> *o):
            max_X(x), max_Y(y), max_Z(z){
                obstacles = *o;

                Construct();
            }
        
        bool InBounds(Coordinate *c){
            return (0<=c->x && c->x < max_X) && (0<=c->y && c->y < max_Y) && (0<=c->z && c->z < max_Z);
        }

        void AddObstacle(Shapes *ob){
            obstacles.push_back(ob);
        }
        
        void Construct(){
            //Create Points
            int i = 0;

            //Create 5 points randomly around the map
            while(i < 5){
                Coordinate *temp;
                
                vector<Shapes*>::iterator it = obstacles.begin();
                
                temp = GenerateLocation();
                bool collide = false;
                
                while(it != obstacles.end()){    
                    if((*it)->DetectCollision(temp->x, temp->y, temp->z)){
                        collide = true;
                    }
                ++it;
                }


                if(!collide){
                    locations.push_back(new TNode(*temp));
                    cout<<"Adding " << *temp << endl;
                    ++i;
                }
            }

            //Connect points
            for(vector<TNode*>::iterator i = locations.begin(); i != locations.end(); ++i){
                for(vector<TNode*>::iterator j = locations.begin(); j != locations.end(); ++j){
                    if(i == j)
                        continue;
                    
                    vector<float> distances;
                    distances.push_back((*i)->Distance(*j));

                    std::sort(distances.begin(), distances.end());


                }
                 
                (*i)->state = TNode::status::setup;
            }
        }

        Coordinate *GenerateLocation(){
            int x = rand() % max_X;
            int y = rand() % max_Y;
            int z = rand() % max_Z;

            return new Coordinate(x,y,z);
        }

        Coordinate *GenerateLocation(Coordinate lower, Coordinate higher){
            int x_offset = higher.x - lower.x;
            int y_offset = higher.y - lower.y;
            int z_offset = higher.z - lower.z;

            int x = rand() % x_offset + lower.x;
            int y = rand() % y_offset + lower.y;
            int z = rand() % z_offset + lower.z;

            return new Coordinate(x,y,z);            
        }

};
#endif