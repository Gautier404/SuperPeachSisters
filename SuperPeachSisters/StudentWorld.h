#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <map>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Block;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	
	//Helpers for actor logic
	bool collisionWithBlock(Actor* curActor, int dx = 0, int dy = 0);	//determiens if current actor will collid with 
																		//a block dx & dy are in pixels
	Actor* blockingBlock(Actor* curActor, int dx = 0, int dy = 0);		//returns address of blocking block
	bool overlapWithPeach(Actor* curActor); //return if current actor overlaps with peach

	//Actor adders
	void addGoodie(std::string type, int x, int y);

private:
	//std::map <std::string, std::list<Actor*>> m_actors;
	std::list<Actor*> m_actors;
	Actor* m_Peach;
	void createActor(Level::GridEntry, int col, int row); //given a grid entry create a actor at the correct position
	
	bool overlap(Actor* curActor, Actor* targetActor, int dx = 0, int dy = 0); //determins if two actors will collide

};

#endif // STUDENTWORLD_H_
