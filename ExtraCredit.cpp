#include <iostream>
#include <fstream>
#include <string>

#include "ECMap.h"

#define MAX_V 1
#define MAX_A 0.2
#define MIN_V -1
#define MIN_A -0.2

using namespace std;

int main(int argc, char *argv[]){
    srand(time(NULL));

   ifstream FILE;
    string filePath;
    double v_0 = 1;
    double a_0 = .2;

    string::size_type s;

#pragma region Argument_Handling
    if(argc >= 2){
        filePath = argv[1];
        v_0 = stod(argv[2], &s);
        a_0 = stod(argv[3], &s);

        if(argc >4){
            cout<<"Too many arguments"<<endl
            <<"Enter 1 argument with the file path or no argument for world.txt"<<endl;
            return -1;
        }
    }
    else{
        filePath = "world.txt";
    }

    if(v_0 > MAX_V || v_0 < MIN_V){
        v_0 = MAX_V;
    }

    if(a_0 > MAX_A || a_0 < MIN_A){
        a_0 = MAX_A;
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

    ECMap tree = ECMap(Xmax, Ymax, Zmax);

    int x, y, z, s;

    //Fills in the obstacles
    for(int i = 0; i < 4; ++i){
        
        FILE >> x >> y >> z >> s;

        if(i < 2){
            tree.AddObstacle(new Cube(x, y, z, s));
        }
        else{
            //Spheres are treated as cubes
            tree.AddObstacle(new Sphere(x, y, z, s));
        }
    }

    int startx, starty, startz;
    int endx, endy, endz;

    FILE>>startx>>starty>>startz>>endx>>endy>>endz;
    FILE.close();

    Coordinate start = Coordinate(startx, starty, startz);
    Coordinate end = Coordinate(endx, endy, endz);

    TStack* path = tree.FindPath(start, end, 3000);

    ofstream outfile;
    outfile.open("ExtraCredit.txt");

    if(!outfile){
        cout<<"Could not open the file, here is the path"<<endl
            <<*path << endl;
        return -1;
    }

    RowVector r_x = RowVector();
    r_x.column[0] = startx;
    r_x.column[1] = v_0;
    r_x.column[2] = endx;
    r_x.column[3] = 0;

    RowVector r_y = RowVector();
    r_y.column[0] = starty;
    r_y.column[1] = v_0;
    r_y.column[2] = endy;
    r_y.column[3] = 0;

    RowVector r_z = RowVector();
    r_z.column[0] = startx;
    r_z.column[1] = v_0;
    r_z.column[2] = endx;
    r_z.column[3] = 0;

    Polynomial q_x = Polynomial(0);
    Polynomial v_x = q_x.Derive();
    

    outfile << *path;
    outfile.close();

    path->Clear();
    cout<<"Completed"<<endl;
    return 0;


    return 0;
}