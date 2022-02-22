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
		startX, // column first - x //TODO check to make sure I'm doing this math right
		startY, // then row - y
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

bool Actor::damagable() const{
	return false;
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

//-------------Peach----------------//
Peach::Peach(StudentWorld* world, int startX, int startY):
	Actor(world, IID_PEACH, startX, startY, 0, 0, 1) {
	ticksOfInvincibility = 0;
	ticksOfTempInvincibility = 0;
	invincible = false;
	tempInvincible = false;
	jumpDistToGo = 0;
	goodieBag.jump = false;
	goodieBag.star = false;
	goodieBag.shoot = false;
	hitpoints = 1;
};

void Peach::doSomething() {
	//1. Check to see if she is alive
	//	if not then return immediatly
	if (!ifAlive()) return;
	//2. Check if she is currently invincible
	//	if so decrement ticks before she loses
	//	invincibility and change status
	if (invincible) {
		ticksOfInvincibility--;
		if (ticksOfInvincibility <= 0) invincible = false;
	}
	//3. Check to see if she's currently temp Invincible
	if (tempInvincible) {
		ticksOfTempInvincibility--;
		if (ticksOfTempInvincibility <= 0) tempInvincible = false;
}
	//4. Check if she is in recharge mode
	if (recharge) {
		ticksOfrecharge--;
		if (ticksOfrecharge <= 0) recharge = false;
	}
	//5. Check to see if she overlaps with another object and bonk
	//TODO

	//6. Do jump stuff
	if (jumpDistToGo > 0) continueJump();
	//7. Do falling stuff if not jumping
	else fall();
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
		case KEY_PRESS_UP:
			initJump();
			break;
		}
	}
	
	
	return;//dummy
};

void Peach::bonk() {
	return;//dummy
}

bool Peach::damagable() const{
	return true;
}

bool Peach::hasStar() const{
	return goodieBag.star;
}

bool Peach::hasJump() const {
	return goodieBag.jump;
}

bool Peach::hasShoot() const {
	return goodieBag.shoot;
}

void Peach::giveJump() {
	goodieBag.jump = true;
}

void Peach::giveShoot() {
	goodieBag.shoot = true;
}

void Peach::giveStar() {
	goodieBag.star = true;
}

void Peach::setHitPoint(int hp) {
	hitpoints = hp;
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

//bonk object on peaches right or move to the right
void Peach::moveRight() {
	setDirection(0);
	if (getWorld()->collisionWithBlock(this, +4)) {
		//TODO bonk that object
		return;
	}
	moveTo(getX() + 4, getY());
}

//if peach has something solid below her set her jump distance
//based on her mushroom power up
void Peach::initJump() {
	if (!getWorld()->collisionWithBlock(this, 0, -1))return;
	if (goodieBag.jump) jumpDistToGo = 12;
	else jumpDistToGo = 8;
	getWorld()->playSound(SOUND_PLAYER_JUMP);
}

//if peach is currently jumping bonk actor above her or move her up
void Peach::continueJump() {
	if (jumpDistToGo <= 0) return;
	Actor* blockToBonk = getWorld()->blockingBlock(this, 0, +4);
	if (blockToBonk != nullptr) {
		blockToBonk->bonk();//TODO bonk object above peach
		jumpDistToGo = 0;
		return;
	}
	moveTo(getX(), getY() + 4);
	jumpDistToGo--;
}

//check if an object (0, 3] pixels below her
//if no object translate her y by -4 pixels
void Peach::fall() {
	if (getWorld()->collisionWithBlock(this, 0, -3))return; //TODO does this actually prevent falling
	moveTo(getX(), getY() - 4);
}
//returns if peach is invincible
bool Peach::isInvincible() {
	return invincible || tempInvincible;
}
//-------------Block----------------//
Block::Block(StudentWorld* world, int startX, int startY, string goodie):
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
	//if block empty or already gave its goodie play bonk sound
	if (m_goodie == "none" || m_beenBonked == true) getWorld()->playSound(SOUND_PLAYER_BONK);
	else {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		getWorld()->addGoodie(m_goodie, getX(), getY());
		m_beenBonked = true;
	}
}



//-----------Goodie----------------//
Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY):
	Actor(world, imageID, startX, startY, 0, 1, 1) {}

void Goodie::doSomething() {
	//check if overlap with peach 
	//if overlap with peach power her up
	if (getWorld()->overlapWithPeach(this)) powerPeachUp();
	
	//fall if goodie can
	fall();
	//patrol across game
	patrol();
	return;
}

void Goodie::patrol() {
	if (getDirection() == 0) {
		//if facing right check if goodie can move to the right.
		//move it if it can or switch its direction
		if (!getWorld()->collisionWithBlock(this, +2)) moveTo(getX() + 2, getY());
		else setDirection(180);
	}
	else {
		//if facing left check if goodie can move to the left.
		//move it if it can or switch its direction.
		if (!getWorld()->collisionWithBlock(this, -2)) moveTo(getX() - 2, getY());
		else setDirection(0);
	}
}

void Goodie::fall() {
	if (getWorld()->collisionWithBlock(this, 0, -2))return; //TODO does this actually prevent falling
	moveTo(getX(), getY() - 2);
}

void Goodie::bonk() {
	return;//TODO do I really need this? Maybe reconsider making bonk pure virtual for actor
}
//-----------Mushroom--------------//
Mushroom::Mushroom(StudentWorld* world, int startX, int startY) :
	Goodie(world, IID_MUSHROOM, startX, startY) {};

void Mushroom::powerPeachUp() {
	getWorld()->increaseScore(75);
	//inform peach she has jump
	getWorld()->getPeach()->giveJump(); //this and the score are the only thing that change so maybe further simplify
	//add peach hitpoint
	getWorld()->getPeach()->setHitPoint(2);
	kill();
	getWorld()->playSound(SOUND_PLAYER_POWERUP);
	return;
}