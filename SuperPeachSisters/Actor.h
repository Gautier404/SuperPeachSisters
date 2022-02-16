#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor :public GraphObject {
public:
	Actor(StudentWorld* world, 
		int imageID,
		int startX, // column first - x
		int startY, // then row - y
		int startDirection,
		int depth,
		double size);
	virtual void doSomething() = 0;
	virtual void bonk() = 0; //removes one hit point
	bool ifAlive() const; //returns if actor is alive
	void kill(); // sets isAlive to false
	virtual bool canMoveThrough(); //returns if able to move through actor
	StudentWorld* getWorld() const;
private:
	bool isAlive;
	StudentWorld* m_world;
};

class Peach :public Actor {
public:
	Peach(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual void bonk();
private:
	bool isInvincible;
	struct Powers {
		bool jump;
		bool shoot;
		bool star;
	};
	Powers m_powers;

	//Helper movement functions
	void moveLeft();
	void moveRight();
};

class Block :public Actor {
public:
	enum Goodie {
		none, star, jump, shoot
	};
	Block(StudentWorld* world, int startX, int startY, Goodie goodie);
	virtual void doSomething();
	virtual void bonk();
	virtual bool canMoveThrough();
	
private:
	Goodie m_goodie;
	//either none, star, jump, or shoot
	bool m_beenBonked;
};

class Enemy : public Actor {

};

class Projectile : public Actor {

};
#endif // ACTOR_H_
