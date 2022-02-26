#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

const std::string MUSHROOM = "jump";
const std::string STAR = "star";
const std::string FLOWER = "shoot";
const std::string NONE = "none";

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
	virtual bool damagable() const;
	void kill(); // sets isAlive to false
	virtual bool canMoveThrough(); //returns if able to move through actor
	StudentWorld* getWorld() const;
	void itemFall(); //translates actor down 2 px if possible;
private:
	bool isAlive;
	StudentWorld* m_world;
};

class Peach :public Actor {
public:
	Peach(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual void bonk();
	virtual bool damagable() const; //Spec needs this for block so I guess I'll need it for most things?
	bool isInvincible(); //returns if peach is currently invincible

	//goodie bag getters
	bool hasStar() const;
	bool hasJump() const;
	bool hasShoot() const;

	//goodie bag setters
	void giveStar();
	void giveJump();
	void giveShoot();

	void setHitPoint(int hp);
private:
	int hitpoints;

	//counters and statuses
	int ticksOfTempInvincibility;
	bool tempInvincible;
	int ticksOfrecharge;
	bool recharge;
	int jumpDistToGo;

	struct GoodieBag { //helpful struct to hold what powers peach can have
		bool jump;
		bool shoot;
		bool star;
		int ticksOfStar;
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
	Block(StudentWorld* world, int startX, int startY, std::string goodie, const int imageID = IID_BLOCK);
	virtual void doSomething();
	virtual void bonk();
	virtual bool canMoveThrough();
	
private:
	std::string m_goodie;
	//either none, star, jump, or shoot
	bool m_beenBonked;
};

class Pipe :public Block {
public:
	Pipe(StudentWorld* world, int startX, int startY);
	virtual void bonk();
};
//class Projectile : public Actor {
//public:
//	Projectile(StudentWorld* world, int imageID, int startX, int startY, int direction);
//	virtual void doSomething();
//
//};

class Goodie : public Actor {
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY);
	virtual void doSomething();
	virtual void powerPeachUp() = 0;
	virtual void bonk();

private:
	void patrol();
};

class Mushroom : public Goodie {
public:
	Mushroom(StudentWorld* world, int startX, int startY);
	virtual void powerPeachUp();
};

class Star : public Goodie {
public:
	Star(StudentWorld* world, int startX, int startY);
	virtual void powerPeachUp();
};

class Flower : public Goodie {
public:
	Flower(StudentWorld* world, int startX, int startY);
	virtual void powerPeachUp();
};

class Enemy : public Actor {

};


#endif // ACTOR_H_
