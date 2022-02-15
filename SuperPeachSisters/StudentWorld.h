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

private:
	//std::map <std::string, std::list<Actor*>> m_actors;
	std::list<Actor*> m_actors;
	void createActor(Level::GridEntry, int col, int row);
};

#endif // STUDENTWORLD_H_
