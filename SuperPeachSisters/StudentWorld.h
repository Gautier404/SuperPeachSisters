#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <map>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

  //Helpers for actor logic
  bool collisionWithBlock(Actor* curActor, int dx = 0, int dy = 0);//dx & dy are in pixels

private:
	//std::map <std::string, std::list<Actor*>> m_actors;
	std::list<Actor*> m_actors;
	void createActor(Level::GridEntry, int col, int row);
	
	bool overlap(Actor* curActor, Actor* targetActor, int dx = 0, int dy = 0); //determins if two actors will collide

};

#endif // STUDENTWORLD_H_
