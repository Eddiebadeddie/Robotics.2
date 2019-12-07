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
            out << "("<< c.x << ", " << c.y << ", " << c.z << ")";
            return out;
        }

        void Change(int i, int j, int k){
            x = i;
            y = j;
            z = k;
        }

        Coordinate *Generate(int x_bound, int y_bound, int z_bound){
            x = rand() % x_bound;
            y = rand() % y_bound;
            z = rand() % z_bound;

            return this;
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

/*=================================================================
    TNode
        Structure that holds the coordinate information and the 
        links to the other nodes
=================================================================*/
struct TNode{
    Coordinate c;
    vector<TNode*> locations;

    int MAX_CONNECTIONS = 5;

    //Literally only for the Queue and the Stack
    TNode *n;
    TNode *p;

    enum State{
        none,
        visited,
        explored
    };

    State state;

    //Constructor
    TNode(Coordinate o){
        state = State::none;
        c = o;
        n = NULL;
        p = NULL;
    }

    TNode(Coordinate *o){
        state = State::none;
        c = *o;
        n = NULL;
        p = NULL;
    }

    TNode(Coordinate o, TNode *next, TNode *prev){
        state = State::none;
        c = o;
        n = next;
        p = prev;
    }

    //Adds an edge if one does not exist at the node
    void AddLocation(TNode* loc){
        if(Search(loc)){
            return;
        }

        locations.push_back(loc);
    }

    bool Search(TNode *loc){
        //Prevents Cycles
        if(c == loc->c){
            return true;
        }

        //cout<<"\tSearch "<< loc->c <<": ";
        for(int i = 0; i < locations.size(); ++i){
            //cout<<i<< " ";
            if(loc->c == locations[i]->c){
                //cout<<"found"<<endl<<endl;
                return true;
            }
        }
        //cout<<"not found" << endl<<endl;
        return false;
    }

    double Distance(const TNode *loc1){
        /*
        cout<<"Distance::"<<endl;
        cout<<"\tDistance between "<<*this << " and " << *loc1 << endl;
        cout<<"x = " << c.x << "\t loc->c.x = " << loc1->c.x<<endl
            <<"y = " << c.y << "\t loc->c.y = " << loc1->c.y<<endl
            <<"z = " << c.z << "\t loc->c.z = " << loc1->c.z<<endl;
        */
        int x = loc1->c.x - c.x;
        int y = loc1->c.y - c.y;
        int z = loc1->c.z - c.z;

        /*
        cout<<"\tx = "<< x << endl
            <<"\ty = "<< y << endl
            <<"\tz = " << z << endl;
        */
        x *= x;
        y *= y;
        z *= z;

        /*
        cout<< endl << "\tx = "<< x << endl
            <<"\ty = "<< y << endl
            <<"\tz = " << z << endl;
        */
        double s = sqrt(x+y+z);
        //cout<<"\tDistance between "<<*this << " and " << *loc1 << endl
        //    <<"\t\t"<<s<<endl;

        return sqrt(x + y + z);
    }

    double Distance(Coordinate *c){
        TNode temp = TNode(c);
        return Distance(&temp);
    }

    void Sort(){
        TNode* temp;
        TNode* min = locations [0];
        double min_d = Distance(locations[0]);
        int index = 0;

        for(int i = 0; i < locations.size(); ++i){
            for(int j = i; j < locations.size(); ++j){
                double check = Distance(locations[j]);
                if(check < min_d){
                    min = locations[j];
                    min_d = check;
                    index = j;
                }
            }
            temp = locations[i];
            locations[i] = locations[index];
            locations[index] = temp;
        }
    }

    friend ostream &operator<<(ostream &out, const TNode &n){
        out << n.c;
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
            //cout<<"Queue::Deleting Queue"<<endl;
            Clear();
            free(head);
            free(tail);
            //cout<<"\tDeleted"<<endl;
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
    TQueue-
        A data structure composed of a doubly linked list 
        that contains a Coordinate instance. Contains a 
        head and tail for simpler enqueing and dequeing.
        Literally copied the other queue and modified it
        for the TNodes. Gonna do the same with the stack.
        Don't expect ingenuity.
---------------------------------------------------------*/
class TQueue{
    public:
        TNode *head;
        TNode *tail;

        TQueue(){
            Coordinate dummy = Coordinate(-1,-1,-1);
            
            head = new TNode(dummy);
            tail = new TNode(dummy);

            head->n = tail;
            tail->p = head;
        }

        ~TQueue(){
            //cout<<"Queue::Deleting Queue"<<endl;
            Clear();
            free(head);
            free(tail);
            //cout<<"\tDeleted"<<endl;
        }

        void Enqueue(TNode *n){
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

            //cout<< "Enqueued: " << n->c <<endl;
        }

        void Enqueue(int x, int y, int z){
            Coordinate c = Coordinate(x, y, z);
            TNode *n = new TNode(c, tail, tail->p);
            Enqueue(n);
        }

        void Enqueue(Coordinate location){
            TNode *n = new TNode(location, tail, tail->p);
            Enqueue(n);
        }

        bool Empty(){
            return head->n==tail;
        }

        TNode *Dequeue(){
            TNode *victim = head->n;

            head->n = victim->n;
            victim->n->p = head;

            victim->n = NULL;
            victim->p = NULL;

            //cout<< endl << "Dequeued: " << victim->c << endl << endl;

            return victim;
        }

        void Clear(){
            TNode *cur = head->n;
            TNode *temp = cur;

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
            TNode *cur = head->n;

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
            //cout<<"Stack::Deleting Stack"<<endl;
            Clear();
            free(head);
            //cout<<"\tDeleted"<<endl;
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

/*---------------------------------------------------------
    TStack-
        Told you
---------------------------------------------------------*/
class TStack{
    public:
        TNode *head;

        TStack(){
            Coordinate c = Coordinate();
            head = new TNode(c);
        }

        ~TStack(){
            //cout<<"Stack::Deleting Stack"<<endl;
            Clear();
            free(head);
            //cout<<"\tDeleted"<<endl;
        }

        void Push(TNode *n){
            n->n = head->n;
            head->n = n;
            //cout<<"Pushed: " << n->c<<endl;
        }

        bool Empty(){
            return head->n==NULL;
        }

        TNode *Pop(){
            if(Empty()){
                return NULL;
            }

            TNode *victim = head->n;
            head->n = victim->n;
            victim->n = NULL;
            
            //cout<<"Popped: " << victim->c << endl;

            return victim;
        }

        void Clear(){
            TNode *cur = head->n;
            TNode *temp = cur;

            head->n = NULL;

            while(cur!=NULL){
                cur = cur->n;
                free(temp);
                temp = cur;
            }
        }

        void Print(){
            TNode *cur = head->n;

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
        friend ostream &operator<<(ostream &out, const TStack &s){
            TNode *cur = s.head->n;

            while (cur != NULL){
                out << cur->c << endl;
                cur = cur->n;
            }

            return out;
        }
};

struct PhysicsInfo{
    TNode *t;
    double velocity;
    double acceleration;

    
};

#endif