#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor :public GraphObject {
public:
	Actor(int imageID,
		int startX, // column first - x
		int startY, // then row - y
		int startDirection,
		int depth,
		double size);
	virtual void doSomething() = 0;
	void damage(); //removes one hit point
	bool isAlive(); //returns if actor is alive
private:
	int hitPoints;
};

class Peach :public Actor {
public:
	Peach(int startX, int startY);
	virtual void doSomething();
private:
	bool isInvincible;
	struct Powers {
		bool jump;
		bool shoot;
		bool star;
	};
	Powers m_powers;
};

class Block :public Actor {
public:
	Block(int startX, int startY, std::string goodie);
	virtual void doSomething();
private:
	std::string m_goodie;
	//either none, star, jump, or shoot
	bool m_beenBonked;
};

class Enemy : public Actor {

};

class Projectile : public Actor {

};
#endif // ACTOR_H_
