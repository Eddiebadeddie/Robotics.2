/**********************************************************
    Queue.h             Author: Eduardo Nodarse
                        10/27/2019
 ==========================================================
 -  This header is used to construct the data structures 
    that help the map class...Originally I was just going
    to use a queue, but then I needed a stack, and a
    special coordinate class, and things just got WAY out
    of hand here
**********************************************************/
#include <iostream>
#include <vector>

using namespace std;

#ifndef QUEUE_H
#define QUEUE_H

/*---------------------------------------------------------
    Coordinate-
        Simple class containg an X Y and Z coordinate to 
        keep track of position
---------------------------------------------------------*/
class Coordinate{
    public:
        int x;
        int y;
        int z;

        Coordinate(){
            x = -1;
            y = -1;
            z = -1;
        }

        Coordinate(int Nx, int Ny, int Nz):
            x(Nx), y(Ny), z(Nz){};

        //Overloaded assignment operator
        Coordinate& operator=(const Coordinate &other){
            x = other.x;
            y = other.y;
            z = other.z;

            return *this;
        }

        //Easier to print...operator for debugging
        friend ostream &operator<<(ostream &out, const Coordinate &c){
            out << c.x << ", " << c.y << ", " << c.z;
            return out;
        }

        void Change(int i, int j, int k){
            x = i;
            y = j;
            z = k;
        }
};

//overloaded comparitor for coordinates
bool operator== (const Coordinate &left, const Coordinate &right){
    if(left.x == right.x){
        if(left.y == right.y){
            if(left.z == right.z){
                return true;
            }
        }
    }
    return false;
}

//Same thing as above
bool operator!=(const Coordinate &left, const Coordinate &right){
    return !(left==right);
}

//Needed this for the unordered map. I didn't want to
class CoordinateHash{
    public:
        size_t operator()(const Coordinate& c) const
        { 
            return c.x+c.y+c.z;
        }
};

//Allows for Linked List implementation of Queue and Stack
struct Node{
    Coordinate c;
    Node *n;
    Node *p;

    Node(Coordinate location):
        c(location){
            n = NULL;
            p = NULL;
        }

    Node(Coordinate location, Node *next, Node *prev):
        c(location), n(next), p(prev){}

    friend ostream &operator<< (ostream &out, const Node* n){
        out << "(" << n->c.x << ", " << n->c.y << ", " << n->c.z << ")";
        return out;
    }
};

/*---------------------------------------------------------
    Queue-
        A data structure composed of a doubly linked list 
        that contains a Coordinate instance. Contains a 
        head and tail for simpler enqueing and dequeing
---------------------------------------------------------*/
class Queue{
    public:
        Node *head;
        Node *tail;

        Queue(){
            Coordinate dummy = Coordinate(-1,-1,-1);
            
            head = new Node(dummy);
            tail = new Node(dummy);

            head->n = tail;
            tail->p = head;
        }

        ~Queue(){
            Clear();
            free(head);
            free(tail);
        }

        void Enqueue(Node *n){
            //If the queue is empty
            if(head->n == tail){
                head->n = n;
                n->n = tail;
                n->p = head;
                tail->p = n;
            }
            else{
                tail->p->n=n;
                if(n->p == NULL){
                    n->p=tail->p;
                    n->n = tail;
                }
                tail->p = n;
            }

            //cout<< "Enqueued: " << n<<endl;
        }

        void Enqueue(int x, int y, int z){
            Coordinate c = Coordinate(x, y, z);
            Node *n = new Node(c, tail, tail->p);
            Enqueue(n);
        }

        void Enqueue(Coordinate location){
            Node *n = new Node(location, tail, tail->p);
            Enqueue(n);
        }

        bool Empty(){
            return head->n==tail;
        }

        Node *Dequeue(){
            Node *victim = head->n;

            head->n = victim->n;
            victim->n->p = head;

            victim->n = NULL;
            victim->p = NULL;

            //cout<< endl << "Dequeued: " << victim << endl << endl;

            return victim;
        }

        void Clear(){
            Node *cur = head->n;
            Node *temp = cur;

            head->n = tail;
            tail->p = head;

            while(cur != tail){
                cur = cur->n;
                temp->n = NULL;
                temp->p = NULL;
                free(temp);
                temp = cur;
            }
        }

        void Print(){
            Node *cur = head->n;

            while (cur != tail){
                if(cur != head->n){
                    cout<<"<->";
                }
                cout << "(" << cur->c.x << ", " << cur->c.y <<", " << cur->c.z << ")";
                cur = cur->n;
            }

            cout<<endl;
        }
};

/*---------------------------------------------------------
    Stack-
        A data structure composed of a linked list 
        that contains a Coordinate instance. Contains a 
        head for simpler pushing and popping
---------------------------------------------------------*/
class Stack{
    public:
        Node *head;

        Stack(){
            Coordinate c = Coordinate();
            head = new Node(c);
        }

        ~Stack(){
            free(head);
        }

        void Push(Node *n){
            n->n = head->n;
            head->n = n;
        }

        bool Empty(){
            return head->n==NULL;
        }

        Node *Pop(){
            if(Empty()){
                return NULL;
            }

            Node *victim = head->n;
            head->n = victim->n;
            victim->n = NULL;

            return victim;
        }

        void Clear(){
            Node *cur = head->n;
            Node *temp = cur;

            head->n = NULL;

            while(cur!=NULL){
                cur = cur->n;
                free(temp);
                temp = cur;
            }
        }

        void Print(){
            Node *cur = head->n;

            while (cur != NULL){
                if(cur != head->n){
                    cout<<"->";
                }
                cout << "(" << cur->c << ")";
                cur = cur->n;
            }

            cout<<endl;
        }

        //Allows for printing to file
        friend ostream &operator<<(ostream &out, const Stack &s){
            Node *cur = s.head->n;

            while (cur != NULL){
                out << cur->c << endl;
                cur = cur->n;
            }

            return out;
        }
};
#endif