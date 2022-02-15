#include "Actor.h"
#include "StudentWorld.h"

using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



//------------Graph Object Helpful Member Functions-------------//
//double getX() const; // in pixels (0-255)
//double getY() const; // in pixels (0-255)
//void moveTo(double x, double y); // in pixels (0-255)
//int getDirection() const; // in degrees (0-359)
//void setDirection(int d); // in degrees (0-359)
//void increaseAnimationNumber(); // forces a sprite to animate frames



//--------------Actor----------------//
Actor::Actor(int imageID,
	int startX, // column first - x
	int startY, // then row - y
	int startDirection,
	int depth = 0,
	double size = 1.0) :
	GraphObject(imageID,
		startX, // column first - x
		startY, // then row - y
		startDirection,
		depth,
		size) {
	hitPoints = 1;
}


void Actor::damage() {
	hitPoints -= 1;
}

//-------------Peach----------------//
Peach::Peach(int startX, int startY):
	Actor(IID_PEACH, startX, startY, 0) {
	isInvincible = false;
	m_powers.jump = false;
	m_powers.star = false;
	m_powers.shoot = false;
};

void Peach::doSomething() {
	return;
	//dummy
};

//-------------Block----------------//
Block::Block(int startX, int startY, string goodie):
	Actor(IID_BLOCK, startX, startY, 0, 2) {
	m_goodie = goodie;
	m_beenBonked = false;
}

void Block::doSomething() {
	return; //dummy
}
