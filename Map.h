/**********************************************************
    Map.h               Author: Eduardo Nodarse
                        10/27/2019
 ==========================================================
 -  This header is used to construct the map that is being
    traversed. The map takes in the upper and lower bounds
    for the coordinates and determines the size of the 
    3D array based off that data. The Map is then
    constructed. The map takes care of adding in the 
    obstacles, preparing for the motion planning, and 
    actually doing the motion planning
**********************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdlib.h>

#include "Shapes.h"
#include "Queue.h"
#include "Vector3D.h"

using namespace std;

#ifndef MAP_H
#define MAP_H

class Map{
    public:
        int x_lower;
        int x_upper;
        int y_lower;
        int y_upper;
        int z_lower;
        int z_upper;

        //differences of the bounds, for simplicity
        int x;
        int y;
        int z;

        int ***map;

        //Creates the 3D map
        Map(int x_l, int x_u, int y_l, int y_u, int z_l, int z_u, int num=0){

            x_lower = x_l;
            x_upper = x_u;
            y_lower = y_l;
            y_upper = y_u;
            z_lower = z_l;
            z_upper = z_u;

            x = x_upper - x_lower;
            y = y_upper - y_lower;
            z = z_upper - z_lower;

            map = new int**[x];

            for(int i = 0; i < x; ++i){
                map[i] = new int*[y];
                for(int j = 0; j < y; ++j){
                    map[i][j] = new int[z];
                    for(int k = 0; k < z; ++k){
                        map[i][j][k] = num;
                    }
                }
            }
        }
        
        //Deletes the map
        ~Map(){
            for(int i = 0; i < x; ++i){
                for(int j = 0; j < y; ++j){
                    for(int k = 0; k < z; ++k){
                        delete[] map[i][j];
                    }
                    delete[] map[i];
                }
                delete[] map;
            }
        }

        /**************************************************
        *   Add_Obstacle
        *       -Adds an obstacle to the map, represented
        *       as a -1
        **************************************************/
        void Add_Obstacle(Shapes *obstacle){
            for(int i = 0; i < x; ++i){
                //cout<<i<<endl;
                for(int j = 0; j<y; ++j){
                    //cout<<"\t";
                    for(int k = 0; k < z; ++k){
                        if(obstacle->DetectCollision(i,j,k)){
                            map[i][j][k] = -1;
                        }
                        //cout<< map[i][j][k] << " ";
                    }
                    //cout<<endl;
                }
            }
        }

        /**************************************************
        *   PrepPath
        *       -Adds a heuristic to the map to allow for
        *       greedy algorithm to take effect
        **************************************************/
        void PrepPath(Coordinate end){
            Queue q = Queue();
            q.Enqueue(end);     //Works backwards

            //Until all the locations have a wieght
            while(!q.Empty()){
                Node *cur = q.Dequeue();
                Coordinate temp = cur->c;
                free(cur);

                //Add a weight of 1 to the first cell
                if(map[temp.x][temp.y][temp.z] == 0){
                    map[temp.x][temp.y][temp.z] = 1;
                }

                //Explore around the current cell
                for(int j = temp.y - 1; j <= temp.y +1; ++j){
                    for(int i = temp.x - 1; i <= temp.x + 1; ++i){
                        for(int k = temp.z - 1; k <= temp.z + 1; ++k){
                            if(InBounds(i, j, k) && map[i][j][k] == 0){
                                //Adds 1 to surrounding cells
                                map[i][j][k] = map[temp.x][temp.y][temp.z] + 1;
                                q.Enqueue(i, j, k);
                            }
                        }
                    }
                }
            }

            //Print();   
        }

        /**************************************************
        *   PlanPath
        *       -Returns a stack of locations that
        *       represents a path to the end location from
        *       a starting location 
        **************************************************/
        Stack *PlanPath(Coordinate start, Coordinate end){
            PrepPath(end);      //Add heuristic to map

            Queue q = Queue();
            q.Enqueue(start);

            //Y'know, THE Tomb Raider. She explores
            Coordinate Lara = start;
            
            Node *cur;

            //Keeps track of the path
            unordered_map<Coordinate, Coordinate, CoordinateHash> umap;

            /*Until Lara reaches the ending destination,
              explore the surrounding cells to determine
               which are good candidates for a path*/
            while(Lara != end){
                if(q.Empty()){
                    cerr<<"This map contains no path to the end"<<endl;
                    exit(EXIT_FAILURE);
                }

                //Incrementation
                cur = q.Dequeue();
                Lara = cur->c;
                //cout<<endl<<"Dequeued: "<<Lara<<endl<<endl;
                free(cur);
                for(int j = Lara.y - 1; j <= Lara.y +1; ++j){
                    for(int i = Lara.x - 1; i <= Lara.x + 1; ++i){
                        for(int k = Lara.z - 1; k <= Lara.z + 1; ++k){
                            /*If the position is in the 
                              map, and the value of the
                              map at that particular 
                              location is less than
                              Lara's location and
                              positive, that cell is a
                              candidate for the final
                              path*/
                            if(InBounds(i, j, k) && (map[i][j][k] < map[Lara.x][Lara.y][Lara.z]) && (map[i][j][k] > 0)){
                                Coordinate temp = Coordinate(i,j,k);
                                if(umap.find(temp) == umap.end()){
                                    q.Enqueue(temp);
                                    //cout<<"Enqueued: "<<temp<<endl;
                                    umap[temp] = Lara;
                                }
                            }
                        }
                    }
                }

            }
            //Clears the Queue of no longer needed memory
            q.Clear();

            //Creates the stack that is to be returned
            Stack *stack = new Stack();

            /*Backtracks through the unordered map to get
              the path*/
            Node *temp = new Node(end);
            stack->Push(temp);

            Lara = umap[end];

            while(Lara != start){
                temp = new Node(umap[Lara]);
                stack->Push(temp);
                Lara = umap[Lara];
            }
            return stack;
        }

        /**************************************************
        *   InBounds
        *       -Checks to see if the point is in the map
        *       based on the coordinate
        ************************************************/  
        bool InBounds(Coordinate c){
            if(x_lower<=c.x && c.x < x_upper){
                if(y_lower<=c.y && c.y < y_upper){
                    if(z_lower<=c.z && c.z < z_upper){
                        return true;
                    }
                }
            }
            return false;
        }

        bool InBounds(int x, int y, int z){
            if(x_lower<=x && x < x_upper){
                if(y_lower<=y && y < y_upper){
                    if(z_lower<=z && z < z_upper){
                        return true;
                    }
                }
            }
            return false;
        }

        /**************************************************
        *   Print
        *       -Prints the map in a...coherent(?) way
        *************************************************/ 
        void Print(){
            for(int i = 0; i < x; ++i){
                cout<<i<<endl;
                for(int j=0; j<y; ++j){
                    cout<<"\t";
                    for(int k=0; k<z; ++k){
                        if(map[i][j][k] < 10 && map[i][j][k] > 0)
                            cout<<"0";
                        cout<<map[i][j][k] << " ";
                    }
                    cout<<endl;
                }
            }
        }
};

#endif