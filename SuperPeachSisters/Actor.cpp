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

void Actor::bonk() {
	return;//by default bonk does nothing
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

void Actor::itemFall() {
	if (getWorld()->bonkBlockingBlock(this, 0, -2))return; //TODO does this actually prevent falling
	moveTo(getX(), getY() - 2);
}

//-------------Peach----------------//
Peach::Peach(StudentWorld* world, int startX, int startY):
	Actor(world, IID_PEACH, startX, startY, 0, 0, 1) {
	ticksOfTempInvincibility = 0;
	tempInvincible = false;
	recharge = false;
	time_to_recharge_before_next_fire = 0;
	jumpDistToGo = 0;
	goodieBag.jump = false;
	goodieBag.star = false;
	goodieBag.ticksOfStar = 0;
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
	if (goodieBag.star) {
		goodieBag.ticksOfStar--;
		if (goodieBag.ticksOfStar <= 0) goodieBag.star = false;
	}
	//3. Check to see if she's currently temp Invincible
	if (tempInvincible) {
		ticksOfTempInvincibility--;
		if (ticksOfTempInvincibility <= 0) tempInvincible = false;
}
	//4. Check if she is in recharge mode
	if (recharge) {
		time_to_recharge_before_next_fire--;
		if (time_to_recharge_before_next_fire <= 0) recharge = false;
	}
	//5. Check to see if she overlaps with another object and bonk
	getWorld()->bonkOverlapsWithPeach();

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
		case KEY_PRESS_SPACE:
			shoot();
			break;
		}
	}
	
	
	return;//dummy
};

void Peach::bonk() {
	kill();
	return;
}

void Peach::kill() {
	if (goodieBag.star || tempInvincible)return;
	hitpoints--;
	ticksOfTempInvincibility = 10;
	tempInvincible = true;
	goodieBag.jump = false;
	goodieBag.shoot = false;
	if (hitpoints >= 1) getWorld()->playSound(SOUND_PLAYER_HURT);
	if (hitpoints <= 0) Actor::kill();
	return;//TODO dummy
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
	goodieBag.ticksOfStar = 150;
}

void Peach::setHitPoint(int hp) {
	hitpoints = hp;
}

//Peach Helper movement functions

//bonk object on peaches left or move to the left
void Peach::moveLeft() {
	setDirection(180);
	if (getWorld()->bonkBlockingBlock(this, -4)) {
		//TODO bonk that object
		return;
	}
	moveTo(getX() - 4, getY());
}

//bonk object on peaches right or move to the right
void Peach::moveRight() {
	setDirection(0);
	if (getWorld()->bonkBlockingBlock(this, +4)) {
		//TODO bonk that object
		return;
	}
	moveTo(getX() + 4, getY());
}

//if peach has something solid below her set her jump distance
//based on her mushroom power up
void Peach::initJump() {
	if (!getWorld()->bonkBlockingBlock(this, 0, -1))return;
	if (goodieBag.jump) jumpDistToGo = 12;
	else jumpDistToGo = 8;
	getWorld()->playSound(SOUND_PLAYER_JUMP);
}

//if peach is currently jumping bonk actor above her or move her up
void Peach::continueJump() {
	if (jumpDistToGo <= 0) return;
	if (getWorld()->bonkBlockingBlock(this, 0, +4, true) == true) {
		jumpDistToGo = 0;
		return;
	}
	moveTo(getX(), getY() + 4);
	jumpDistToGo--;
}

//check if an object (0, 3] pixels below her
//if no object translate her y by -4 pixels
void Peach::fall() {
	if (getWorld()->bonkBlockingBlock(this, 0, -3))return; //TODO does this actually prevent falling
	moveTo(getX(), getY() - 4);
}

void Peach::shoot() {
	if (!recharge && goodieBag.shoot) {
		getWorld()->playSound(SOUND_PLAYER_FIRE);
		time_to_recharge_before_next_fire = 8;
		recharge = true;
		int dx;
		if (getDirection() == 0) dx = 4;
		else dx = -4;
		getWorld()->addProjectile(PEACHFIRE, getX() + dx, getY(), getDirection());
	}
}


//-------------Block----------------//
Block::Block(StudentWorld* world,  int startX, int startY, string goodie, const int imageID ):
	Actor(world, imageID, startX, startY, 0, 2, 1) {
	m_goodie = goodie;
	m_beenBonked = false;
}

void Block::doSomething() {
	return; //block doesn't do anything
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

//------------Pipe-----------------//
Pipe::Pipe(StudentWorld* world, int startX, int startY) :
	Block(world, startX, startY, NONE, IID_PIPE) {};

void Pipe::bonk() {
	return;//do nothing if bonked
}


//-----------Goodie----------------//
Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY):
	Actor(world, imageID, startX, startY, 0, 1, 1) {}

void Goodie::doSomething() {
	//check if overlap with peach 
	//if overlap with peach power her up
	if (getWorld()->overlapWithPeach(this)) {
		powerPeachUp();
		//set peach hitpoint to 2
		getWorld()->setPeachHPTo(2);
		kill();
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		return;
	}
	//fall if goodie can
	itemFall();
	//patrol across game
	patrol();
	return;
}

void Goodie::patrol() {
	if (getDirection() == 0) {
		//if facing right check if goodie can move to the right.
		//move it if it can or switch its direction
		if (!getWorld()->bonkBlockingBlock(this, +2)) moveTo(getX() + 2, getY());
		else setDirection(180);
	}
	else {
		//if facing left check if goodie can move to the left.
		//move it if it can or switch its direction.
		if (!getWorld()->bonkBlockingBlock(this, -2)) moveTo(getX() - 2, getY());
		else setDirection(0);
	}
}

//-----------Mushroom--------------//
Mushroom::Mushroom(StudentWorld* world, int startX, int startY) :
	Goodie(world, IID_MUSHROOM, startX, startY) {};

void Mushroom::powerPeachUp() {
	//increase score
	getWorld()->increaseScore(75);
	//inform peach she has jump
	getWorld()->givePeachPower(MUSHROOM);
	
	return;
}

//----------Star-------------------//
Star::Star(StudentWorld* world, int startX, int startY) :
	Goodie(world, IID_STAR, startX, startY) {};

void Star::powerPeachUp() {
	//increase score
	getWorld()->increaseScore(100);
	//inform peach she has star
	getWorld()->givePeachPower(STAR);
}


//---------Flower----------------//
Flower::Flower(StudentWorld* world, int startX, int startY) :
	Goodie(world, IID_FLOWER, startX, startY) {};

void Flower::powerPeachUp() {
	//increase score
	getWorld()->increaseScore(50);
	//inform peach she has star
	getWorld()->givePeachPower(FLOWER);
}


//---------Projectile-----------//
Projectile::Projectile(StudentWorld* world, int imageID, int startX, int startY, int direction):
	Actor(world, imageID, startX, startY,direction, 1, 1) {};

void Projectile::doSomething() {
	//check to see if it overlaps with peach or an enemy
	if (hitThing()) {
		kill();
		return;
	}

	//fall if the projectile can fall 
	itemFall();
	//projectile patrol (same for all projectiles)
	if (getDirection() == 0) {
		//if facing right check if goodie can move to the right.
		//move it if it can, otherwise set its status to not alive
		if (!getWorld()->bonkBlockingBlock(this, +2)) moveTo(getX() + 2, getY());
		else kill();
	}
	else {
		//if facing left check if goodie can move to the left.
		//move it if it can, otherwise set its status to not alive
		if (!getWorld()->bonkBlockingBlock(this, -2)) moveTo(getX() - 2, getY());
		else kill();
	};
}

bool Projectile::hitThing() {
	return getWorld()->damageOverlap(this);
}

//--------Peach Fireball--------//
PeachFireball::PeachFireball(StudentWorld* world, int startX, int startY, int direction) :
	Projectile(world, IID_PEACH_FIRE, startX, startY, direction)
{};


Shell::Shell(StudentWorld* world, int startX, int startY, int direction) :
	Projectile(world, IID_SHELL, startX, startY, direction) 
{};

PiranhaFireball::PiranhaFireball(StudentWorld* world, int startX, int startY, int direction) :
	Projectile(world, IID_PIRANHA_FIRE, startX, startY, direction)
{};

bool PiranhaFireball::hitThing() {
	if (getWorld()->overlapWithPeach(this)) {
		getWorld()->hitPeach();
		return true;
	}
	else return false;
}

//-------Flag------------------//
Flag::Flag(StudentWorld* world, int startX, int startY, int imageID) :
	Actor(world, imageID, startX, startY, 0, 1, 1) {};

void Flag::doSomething() {
	if (!ifAlive()) return;
	if (getWorld()->overlapWithPeach(this)) {
		getWorld()->increaseScore(1000);
		kill();
		tellWorldAboutWin();
	}
}

void Flag::tellWorldAboutWin() {
	getWorld()->completeLevel();
}

//------------Mario---------------//
Mario::Mario(StudentWorld* world, int startX, int startY) :
	Flag(world, startX, startY, IID_MARIO) {};

void Mario::tellWorldAboutWin() {
	getWorld()->completeGame();
}

//--------------Enemy------------//
Enemy::Enemy(StudentWorld* world, int imageID, int startX, int startY):
	Actor(world, imageID, startX, startY, 0, 0, 1) {};

bool Enemy::damagable() const {
	return true;
}

void Enemy::doSomething() {
	//if dead return immediatly
	if (!ifAlive()) return;
	//attempt to bonk peach
	if (getWorld()->overlapWithPeach(this)) getWorld()->hitPeach();
	//move if koopa or goomba move or attempt to shoot peach & update animation frame if Piranha
	patrol();
}

void Enemy::patrol() {
	//if enemy is blocked in current direction switch its direction
	if (getDirection() == 0 && getWorld()->bonkBlockingBlock(this, +1)) setDirection(180);
	else if (getWorld()->bonkBlockingBlock(this, -1)) setDirection(0);
	//if not blocked but it will end up moving over empty space switch its direction
	else {
		if (getDirection() == 0 && !getWorld()->isSupported(this, +1)) setDirection(180);
		else if (!getWorld()->isSupported(this, -1)) setDirection(0);
	}
	//if enemy can move in new direction move otherwise immediatly return
	if (getDirection() == 0) {
		if (!getWorld()->bonkBlockingBlock(this, +1)) moveTo(getX() + 1, getY());
		else return;
	}
	else {
		if (!getWorld()->bonkBlockingBlock(this, -1)) moveTo(getX() - 1, getY());
		else return;
	}
}

void Enemy::bonk() {
	if (getWorld()->peachHasStar()) {
		getWorld()->playSound(SOUND_PLAYER_KICK);
		kill();
	}
	return;//TODO
}

void Enemy::kill() {
	getWorld()->increaseScore(100);
	Actor::kill();
	return;//TODO
}

//------------Piranha------------//
Piranha::Piranha(StudentWorld* world, int startX, int startY) :
	Enemy(world, IID_PIRANHA, startX, startY),m_firingDelay(0) {};

void Piranha::patrol() {
	//cycle animation
	increaseAnimationNumber();
	//determine if on same plane of peach
	if (abs(getY() - getWorld()->getPeachY()) >= 1.5 * SPRITE_HEIGHT) return;
	//face peach
	int diff = getX() - getWorld()->getPeachX();
	if (diff > 0) setDirection(180);
	else setDirection(0);
	//deacrease firing delay if neccesary
	if (m_firingDelay > 0) m_firingDelay--;
	//choose to fire
	else {
		if (abs(diff) < 8 * SPRITE_WIDTH)//coordinate is strictly less than 8 * SPRITE_WIDTH pixels away
		{
			getWorld()->addProjectile(PIRANHAFIRE, getX(), getY(), getDirection());
			getWorld()->playSound(SOUND_PIRANHA_FIRE);
			m_firingDelay = 40;
		}
	}
	/*It will compute the distance between itselfand Peach.If Peach’s x
		 from
		Piranha’s x coordinate, then Piranha will :
	i.Add a new Piranha Fireball object to StudentWorld at its current
		x, y position, with an initial direction facing in the same direction
		that Piranha is facing
		ii.Play the sound SOUND_PIRANHA_FIRE using GameWorld’s
		playSound() method.
		iii.Set its firing delay to 40, so Piranha won’t fire at Peach for another
		40 ticks*/
	return; //TODO implement
}

//------------Koopa--------------//
Koopa::Koopa(StudentWorld* world, int startX, int startY) :
	Enemy(world, IID_KOOPA, startX, startY) {};

void Koopa::kill() {
	//if the koopas already been killed don't do anything
	Enemy::kill();
	getWorld()->addProjectile(SHELL, getX(), getY(), getDirection());
	return; //TODO implement shell thing
}

//------------Goomba-------------//
Goomba::Goomba(StudentWorld* world, int startX, int startY) :
	Enemy(world, IID_GOOMBA, startX, startY) {};


