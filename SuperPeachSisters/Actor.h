#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor :public GraphObject{
public:
	Actor(int imageID,
		 int startX, // column first - x
		 int startY, // then row - y
		 int startDirection,
		 int depth = 0,
		 double size = 1.0);
	void doSomething();
	void damage(); //removes one hit point
	bool isAlive(); //returns if actor is alive
private:
	int hitPoints; 
}

class Peach :public Actor {

};

class Block :public Actor {

};

class Enemy : public Actor {

};

class Projectile : public Actor {

};
#endif // ACTOR_H_
