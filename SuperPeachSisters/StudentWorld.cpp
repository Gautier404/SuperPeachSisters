#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

//--------------Game World Functions-------------//
//int getLevel() const;
//int getLives() const;
//void decLives();
//void incLives();
//int getScore() const;
//void increaseScore(int howMuch);
//void setGameStatText(string text);
//bool getKey(int& value);
//void playSound(int soundID);

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {

};

int StudentWorld::init()
{
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        Level::GridEntry ge;
        for (int col = 0; col < GRID_WIDTH; col++) {
            for (int row = 0; row < GRID_HEIGHT; row++) {
                ge = lev.getContentsOf(col, row);
                createActor(ge, col, row);
            }
        }

    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    return GWSTATUS_CONTINUE_GAME;
    //decLives();
    //return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}




//-------------Helpers---------------//
void StudentWorld::createActor(Level::GridEntry ge, int col, int row) {
    //Options
    //empty, peach, koopa, goomba, piranha, block, star_goodie_block,
    //mushroom_goodie_block, flower_goodie_block, pipe, flag, mario
    switch (ge)
    {
    case Level::empty:
        break;
    case Level::peach: {
        Actor* newPeach = new Peach(col, row);
        m_actors.push_front(newPeach);
        break;
    }
        
    case Level::block:
    case Level::mushroom_goodie_block:
    case Level::flower_goodie_block:
    case Level::star_goodie_block: {
        Actor* newBlock = new Block(col, row, Block::none);
        m_actors.push_back(newBlock);
        break;
    }
        
    }
}

