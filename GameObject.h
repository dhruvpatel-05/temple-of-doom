//
//  GameObject.h
//  Proj3
//
//  Created by Dhruv Patel on 5/20/24.
//

#ifndef GameObject_h
#define GameObject_h
#include <string>
using namespace std;

class GameObject {
public:
    GameObject(const string& n) : name(n) {}
    string getName() const {return name;}
    virtual ~GameObject() {};
private:
    string name;
};

#endif /* GameObject_h */
