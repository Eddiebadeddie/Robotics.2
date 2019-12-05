#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unordered_map>

#include "Shapes.h"
#include "Queue.h"
#include "Vector3D.h"

using namespace std;

#ifndef RRTMAP_H
#define RRTMAP_H

class RRTMap{
    public: 
        int x;
        int y;
        int z;

        unsigned int SAMPLES = 3000;

        vector<Shapes*> obstacles;

        vector<TNode*> tree;

        RRTMap(int hx, int hy, int hz):
        x(hx), y(hy), z(hz){
            //cout<<"x: "<<x << endl
            //    <<"y: "<<y << endl
            //    <<"z: "<<z << endl;
        }

        ~RRTMap(){
            int num = tree.size();
            for(int i = 0; i < num; ++i){
                free(tree[i]);
            }
        }

        void AddObstacle(Shapes* s){
            obstacles.push_back(s);
        }

        void RRT(TNode *start, TNode *end){
            //cout<<"RRT::"<<endl;
            if(DetectCollision(start) || DetectCollision(end)){
                cerr<<"This map contains no path to the end"<<endl;
                    exit(EXIT_FAILURE);
            }

            for (unsigned int i = 0; i < SAMPLES; ++i){
                //cout<<"LOOP===================="<<endl;
                //cout<< i << ":: "<<endl;
                Coordinate *drive = new Coordinate();

                double chance = ((double) rand() / (double) RAND_MAX);
                //cout<<"\tChance = " << chance << endl;
                if(chance <=.2)
                    drive = &(end->c);

                while(!InBounds(drive) || DetectCollision(drive)){
                    //cout<<"\tSum Ting Wong...making a new one::";
                    drive = drive->Generate(x, y, z);
                }
                //cout<<"\tHeaded towards" << *drive<<endl;

                TNode *near = Nearest(drive);
                //cout<<"\tNearest = "<< near->c << endl;
                Vector3D *dir = new Vector3D;
                dir->GetVectorTo(near->c, *drive);

                dir->ConvertToStep();

                int new_x = near->c.x + dir->x;
                int new_y = near->c.y + dir->y;
                int new_z = near->c.z + dir->z;

                free(dir);

                Coordinate newLocation = Coordinate(new_x, new_y, new_z);

                //cout<<"\tTesting new location "<<newLocation;
                if(DetectCollision(&newLocation) || !InBounds(&newLocation)){
                    --i;
                    //cout<<" Failed, restarting iteration"<<endl;
                    continue;
                }
                //cout<<"\tSuccess!"<<endl;

                TNode *newNode;
                if(newLocation != end->c){
                    newNode = new TNode(&newLocation);
                    AddLocation(newNode);
                }
                else
                {
                    //cout<< "This is the end node"<<endl;
                    newNode = end;
                }
                
                newNode->AddLocation(near);
                near->AddLocation(newNode);
                //cout<<"\tAdded and connected " << newNode->c << " to " << near->c <<endl<<endl;
            }

            //PrintTree();
        }

        void AddLocation(TNode *t){
            //cout<<"AddLocation::"<<endl;
            if(Search(t->c)){
                //cout<<"\tLocation Found, Not Adding"<<endl<<endl;
                return;
            }
            else
            {
                //cout<<"\tAdding"<<endl<<endl;
                tree.push_back(t);
            }
            
        }

        bool Search(Coordinate c){
            unsigned int num = tree.size();
            for(unsigned int i = 0; i < num; ++i){
                if(c == tree[i]->c){
                    return true;
                }
            }
            return false;
        }

        TStack *FindPath(Coordinate start, Coordinate end, int num_samples = 1000){
            SAMPLES = num_samples;
            TNode *Start = new TNode(start);
            TNode *End = new TNode(end);

            AddLocation(Start);

            RRT(Start, End);

            AddLocation(End);
            
            TStack *ret = new TStack();

            TQueue q = TQueue();
            q.Enqueue(Start);
            Start->state = TNode::State::visited;

            TNode *cur;
            unordered_map<TNode*, TNode*> map;

            while(!q.Empty()){
                cur = q.Dequeue();

                for(int i = 0; i < cur->locations.size(); ++i){
                    if(cur->locations[i]->state == TNode::State::none){
                        q.Enqueue(cur->locations[i]);
                        cur->locations[i]->state = TNode::State::visited;
                        map[cur->locations[i]] = cur;
                    }
                }
                cur->state = TNode::State::explored;
            }

            cur = End;
            ret->Push(cur);
            //cout<<cur->c<<endl;

            while(cur->c != Start->c){
                ret->Push(map[cur]);
                cur = map[cur];
            }

            //ret->Print();
            //PrintTree();
            return ret;
        }

        bool DetectCollision(TNode* t){
            //cout<<endl<<"\tDetectCollision::";
            for(int i = 0; i < obstacles.size(); ++i){
                //cout<<"\n\t\tTesting obstacle["<<i<<"]: ";
                if(obstacles[i]->DetectCollision(&(t->c))){
                    //cout<<"Collided"<<endl;
                    return true;
                }
            }
            //cout<<endl<<"\t\tNo Collisions Detected"<<endl;
            return false;
        }

        bool DetectCollision(Coordinate *c){
            TNode temp = TNode(c);
            return DetectCollision(&temp);
        }

        TNode *Nearest(TNode* target){
            //cout<<"Nearest::"<<endl;
            double min_dist = tree[0]->Distance(target);
            TNode *min = tree[0];

            //cout<<"\tCurrent min = " << min->c<<endl
            //    <<"\tCurrent min_dist = " << min_dist << endl;
            int num = tree.size();
            for(int i = 0; i < num; ++i){
                double check = target->Distance(tree[i]);
                if(check <= min_dist){
                    min_dist = check;
                    min = tree[i];
                    //cout<<"\tnew min = " << min->c<<endl
                    //    <<"\tnew min_dist = "<<min_dist<<endl;
                }
            }

            return min;
        }

        TNode *Nearest(Coordinate *c){
            TNode temp = TNode(c);
            return Nearest(&temp);
        }

        bool InBounds(TNode *t){
            return InBounds(&(t->c));
        }

        bool InBounds(Coordinate *c){
            //cout<<"InBounds"<<endl;
            bool t_x, t_y, t_z;
            
            if(0<= c->x && c->x < x){
            }
            else
            {
                //cout<<"\tx is out of bounds"<<endl;
                return false;
            }
            
            if(0<= c->y && c->y < y){}
            else
            {
                //cout<<"\ty is out of bounds"<<endl;
                return false;
            }

            if(0<= c->z && c->z < z){
            }
            else
            {
                //cout<<"\tz is out of bounds"<<endl;
                return false;
            }
            
            //cout<<"Coordinate is in bounds"<<endl;
            return true;
        }

        void PrintTree(){
            //cout<<endl<<"RRT MAP"<<endl;
            //cout<<"Nodes>>>>>>>>>>>>>>"<<endl;
            
            for(int i = 0; i < tree.size(); ++i){
                //cout<<"\t"<<i<<")."<<tree[i]->c<<endl;
                for(int j = 0; j < tree[i]->locations.size(); ++j){
                    //cout<<"\t\t"<<j<<")."<<tree[i]->locations[j]->c<<endl;
                }
                //cout<<endl;
            }

            //cout<<endl<<"Obstacles>>>>>>>>>>>>>>"<<endl;

            for(int i = 0; i < obstacles.size(); ++i){
                Coordinate temp = Coordinate(obstacles[i]->x_center, obstacles[i]->y_center, obstacles[i]->z_center);
                //cout<<"\t"<<i<<"). " << temp<<endl;
            }
            //cout<<endl;
        }
};

#endif