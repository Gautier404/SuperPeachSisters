#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

//for create goodie function in student world
const std::string MUSHROOM = "jump";
const std::string STAR = "star";
const std::string FLOWER = "shoot";
const std::string NONE = "none";

//for create projectile function in student world
const int PEACHFIRE = 0;
const int PIRANHAFIRE = 1;
const int SHELL = 2;

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
	virtual void bonk();
	virtual bool isFriendly() const;
	bool ifAlive() const; //returns if actor is alive
	virtual bool damagable() const;
	virtual void kill(); // sets isAlive to false
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
	virtual void kill();
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
	void shoot();
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

class Goodie : public Actor {
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY);
	virtual void doSomething();
	virtual void powerPeachUp() = 0;

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

class Projectile : public Actor {
public:
	Projectile(StudentWorld* world, int imageID, int startX, int startY, int direction);
	virtual void doSomething();
	virtual bool hitThing();
};

class PeachFireball :public Projectile {
public:
	PeachFireball(StudentWorld* world, int startX, int startY, int direction);
};

class Shell :public Projectile {
public:
	Shell(StudentWorld* world, int startX, int startY, int direction);
};

class PiranhaFireball :public Projectile {
public:
	PiranhaFireball(StudentWorld* world, int startX, int startY, int direction);
	virtual bool hitThing();
}; 

class Flag : public Actor {
public:
	Flag(StudentWorld* world, int startX, int startY, int imageID = IID_FLAG);
	virtual void doSomething();
	virtual void tellWorldAboutWin();
};

class Mario : public Flag {
public:
	Mario(StudentWorld* world, int startX, int startY);
	virtual void tellWorldAboutWin();
};

class Enemy : public Actor {
public:
	Enemy(StudentWorld* world, int imageID, int startX, int startY);
	virtual void doSomething();
	virtual bool damagable() const;
	virtual void bonk();
	virtual void kill();
	virtual void patrol();
	virtual bool isFriendly() const;
};

class Piranha : public Enemy {
public:
	Piranha(StudentWorld* world, int startX, int startY);
	virtual void patrol();
private:
	int m_firingDelay;
};

class Koopa : public Enemy {
public:
	Koopa(StudentWorld* world, int startX, int startY);
	virtual void kill();
};

class Goomba : public Enemy {
public:
	Goomba(StudentWorld* world, int startX, int startY);
};


#endif // ACTOR_H_
