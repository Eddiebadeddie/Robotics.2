#include <iostream>
#include <fstream>

#include "PRMTree.h"

using namespace std;

int main(int argc, char *argv[]){
    srand(time(NULL));

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

    PRMTree tree = PRMTree(Xmax, Ymax, Zmax, &obstacles);

    return 0;
}