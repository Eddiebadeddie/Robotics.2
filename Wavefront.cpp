/**********************************************************
    Wavefront.cpp       Author: Eduardo Nodarse
                        10/27/2019
 ==========================================================
 -  This program creates a wavefront graph based on input 
    from a file, which can be specified as an argument
    from the console. This will create a Wavefront graph
    in a 3 dimensional space based on upper and lower 
    bounds specified in the file. The program then will 
    ask for the user's input to determine the starting 
    point and the ending point. The program will then 
    find a path from the starting position to the end 
    position.
**********************************************************/

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#include "Map.h"

using namespace std;

int main(int argc, char *argv[]){
    
    ifstream FILE;
    string filePath;

#pragma region Argument_Handling
    if(argc == 2){
        filePath = argv[1];
    }
    else if(argc > 2){
        cout<<"Too many arguments"<<endl
        <<"Enter 1 argument with the file path or no argument for world.txt"<<endl;
        return -1;
    }
    else{
        filePath = "world.txt";
    }
#pragma endregion Argument_Handling

    FILE.open(filePath);
    
    if(!FILE){
        cout<<"File did not open properly"<<endl;
        return -1;
    }

#pragma region Building Map
    //Bounds of the map
    int Xmin, Xmax;
    int Ymin, Ymax;
    int Zmin, Zmax;

    FILE >> Xmin >> Xmax >> Ymin >> Ymax >> Zmin >> Zmax;

    Map map = Map(Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);

    //cout<<"Map constructed and initialized"<<endl;
    
    int x, y, z, s;

    //Stores obstacles
    vector<Shapes*> obstacles;

    //Fills in the obstacles
    for(int i = 0; i < 4; ++i){
        
        FILE >> x >> y >> z >> s;

        if(i < 2){
            obstacles.push_back(new Cube(x, y, z, s));
        }
        else{
            //Spheres are treated as cubes
            obstacles.push_back(new Sphere(x, y, z, s));
        }
    }

    int startx, starty, startz;
    int endx, endy, endz;

    FILE>>startx>>starty>>startz>>endx>>endy>>endz;
    FILE.close();

    vector<Shapes*>::iterator it = obstacles.begin();

    //Adds obstacles to the map
    for(it; it!=obstacles.end(); ++it){
        map.Add_Obstacle(*it);
    }

    //Checks to see if start or end is in the obstacle area
    for(it = obstacles.begin(); it!=obstacles.end(); ++it){
        if((*it)->DetectCollision(startx, starty, startz)){
            cout<<"ERROR: Invalid starting position"<<endl;
            return -1;
        }

        if((*it)->DetectCollision(endx, endy, endz)){
            cout<<"ERROR: Invalid ending position"<<endl;
            return -1;
        }
    }
#pragma endregion Building Map 

    //Creats a start and ending position for the map
    Coordinate start = Coordinate(startx, starty, startz);
    Coordinate end = Coordinate(endx, endy, endz);

    //Gets the path from the Plan Path function
    Stack *stack = map.PlanPath(start, end);

    ofstream outfile;
    outfile.open("Wavefront.txt");

    if(!outfile){
        cout<<"Could not open the file, here is the path"<<endl
            <<*stack;
        return -1;
    }

    //Prints the path to the file
    outfile<<*stack;
    outfile.close();

    stack->Clear();
    cout<<"Completed"<<endl;
    return 0;
}