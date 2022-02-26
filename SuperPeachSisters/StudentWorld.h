#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <map>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	Peach* getPeach();
	
	//Public Overlapping functions
	bool collisionWithBlock(Actor* curActor, int dx = 0, int dy = 0) const;	//determiens if current actor will collid with 
																			//a block dx & dy are in pixels
	Actor* blockingBlock(Actor* curActor, int dx = 0, int dy = 0) const;		//returns address of blocking block
	bool overlapWithPeach(Actor* curActor) const; //return if current actor overlaps with peach
	bool isSupported(Actor* curActor, int dx) const;
	void bonkOverlapsWithPeach();
	bool damageOverlap(Actor* curActor);


	//level logic setters
	void completeLevel();
	void completeGame();

	//Actor adders
	void addGoodie(std::string type, int x, int y);
	void addProjectile(int type, int x, int y, int direction);
private:
	//std::map <std::string, std::list<Actor*>> m_actors;
	std::list<Actor*> m_actors;
	Peach* m_Peach;

	bool m_levelCompleted;
	bool m_gameCompleted;
	//helper functions for init
	void createActor(Level::GridEntry, int col, int row); //given a grid entry create a actor at the correct position
	
	//helper functions for move
	void updateText();


	bool overlap(Actor* curActor, Actor* targetActor, int dx = 0, int dy = 0) const; //determins if two actors will collide

};

#endif // STUDENTWORLD_H_
