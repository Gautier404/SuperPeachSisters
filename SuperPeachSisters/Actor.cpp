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
Actor::Actor(StudentWorld* world,
	int imageID,
	int startX, // column first - x
	int startY, // then row - y
	int startDirection,
	int depth,
	double size) :
	GraphObject(imageID,
		startX* SPRITE_WIDTH, // column first - x //TODO check to make sure I'm doing this math right
		startY* SPRITE_HEIGHT, // then row - y
		startDirection,
		depth,
		size) {
	isAlive = true;
	m_world = world;
}

bool Actor::ifAlive() const {
	return isAlive;
}

void Actor::kill() {
	isAlive = false;
}

bool Actor::canMoveThrough() {
	return true;
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

//-------------Peach----------------//
Peach::Peach(StudentWorld* world, int startX, int startY):
	Actor(world, IID_PEACH, startX, startY, 0, 0, 1) {
	isInvincible = false;
	goodieBag.jump = false;
	goodieBag.star = false;
	goodieBag.shoot = false;
};

void Peach::doSomething() {
	//1. Check to see if she is alive
	//	if not then return immediatly
	//2. Check if she is currently invincible
	//	if so decrement ticks before she loses
	//	invincibility and change status
	//3. Check to see if she's currently Invincible
	//4. Check if she is in recharge mode
	//5. Check to see if she overlaps with another object
	//6. Do jump stuff
	//7. Do falling stuff
	//8. Keystroke stuff
	int ch;
	if (getWorld()->getKey(ch))
	{
		//9. perform appropriate keystroke action
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			moveLeft();
			break;
		case KEY_PRESS_RIGHT:
			moveRight();
			break;
		}
	}
	
	
	return;//dummy
};

void Peach::bonk() {
	return;//dummy
}

//Peach Helper movement functions

//bonk object on peaches left or move to the left
void Peach::moveLeft() {
	setDirection(180);
	if (getWorld()->collisionWithBlock(this, -4)) {
		//TODO bonk that object
		return;
	}
	moveTo(getX() - 4, getY());
}

void Peach::moveRight() {
	setDirection(0);
	if (getWorld()->collisionWithBlock(this, +4)) {
		//TODO bonk that object
		return;
	}
	moveTo(getX() + 4, getY());
}

//-------------Block----------------//
Block::Block(StudentWorld* world, int startX, int startY, Goodie goodie):
	Actor(world, IID_BLOCK, startX, startY, 0, 2, 1) {
	m_goodie = goodie;
	m_beenBonked = false;
}

void Block::doSomething() {
	return; //dummy
}

bool Block::canMoveThrough() {
	return false;
}

void Block::bonk() {
	return;//dummy
}