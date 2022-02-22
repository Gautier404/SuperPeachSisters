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
	bool isInvincible(); //returns if peach is currently invincible
private:
	int hitpoints;

	//counters and statuses
	int ticksOfInvincibility;
	bool invincible;
	int ticksOfTempInvincibility;
	bool tempInvincible;
	int ticksOfrecharge;
	bool recharge;
	int jumpDistToGo;

	struct GoodieBag { //helpful struct to hold what powers peach can have
		bool jump;
		bool shoot;
		bool star;
	};
	GoodieBag goodieBag;

	//Helper movement functions
	void moveLeft();
	void moveRight();
	void continueJump();
	void initJump();
	void fall();
};

class Block :public Actor {
public:
	Block(StudentWorld* world, int startX, int startY, std::string goodie);
	virtual void doSomething();
	virtual void bonk();
	virtual bool canMoveThrough();
	
private:
	std::string m_goodie;
	//either none, star, jump, or shoot
	bool m_beenBonked;
};

class Goodie : public Actor {
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY);
	virtual void doSomething();
	virtual void powerPeachUp() = 0;
	virtual void bonk();

private:
	void patrol();
	void fall();
};

class Mushroom : public Goodie {
public:
	Mushroom(StudentWorld* world, int startX, int startY);
	virtual void powerPeachUp();
};

class Enemy : public Actor {

};

class Projectile : public Actor {

};
#endif // ACTOR_H_
