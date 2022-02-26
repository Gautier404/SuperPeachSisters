#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
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
    m_Peach = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());

    ostringstream oss;
    if(getLevel() < 10) oss << "level0" << getLevel() << ".txt"; //for when only one digit
    else oss << "level" << getLevel() << ".txt";                    //for two digit levels
    
    string level_file = oss.str();
    //string level_file = "level01.txt";
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
    //Ask all actors to do something
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        (*it)->doSomething();
        //have to add in detection for if peach dies (prolly check if she's alive)
        it++;
    }
    //if peach has reached a flag play SOUND_FINISHED_LEVEL and return GWSTATUS_FINISHED_LEVEL
    //if peach has reached mario play  SOUND_GAME_OVER and return GWSTATUS_PLAYER_WON

    //delete any dead actors
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (!(*it)->ifAlive()) {
            delete (*it);
            it = m_actors.erase(it); //should move iterator to next value
        }
        else it++;
    }
    //update status text
    updateText();

    //if level isn't over then continue the game
    return GWSTATUS_CONTINUE_GAME;


    //decLives();
    //return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        delete (*it);
        it++;
    }
}




//-------------Helpers---------------//
void StudentWorld::createActor(Level::GridEntry ge, int col, int row) {
    //Options
    //empty, peach, koopa, goomba, piranha, block, star_goodie_block,
    //mushroom_goodie_block, flower_goodie_block, pipe, flag, mario
    int x = col * SPRITE_WIDTH;
    int y = row * SPRITE_HEIGHT;
    switch (ge)
    {
    case Level::empty:
        break;
    case Level::peach: {
        Peach* newPeach = new Peach(this,x ,y );
        m_Peach = newPeach;
        Actor* peachButActor = newPeach;
        m_actors.push_front(peachButActor);
        break;
    }
        
    case Level::block: {
        Actor* newBlock = new Block(this, x, y, NONE);
        m_actors.push_back(newBlock);
        break;
    }
    case Level::flower_goodie_block: {
        Actor* newBlock = new Block(this, x, y, FLOWER);
        m_actors.push_back(newBlock);
        break;
    }
    case Level::star_goodie_block: {
        Actor* newBlock = new Block(this, x, y, STAR);
        m_actors.push_back(newBlock);
        break;
    }
    case Level::mushroom_goodie_block: {
        Actor* newBlock = new Block(this, x, y, MUSHROOM);
        m_actors.push_back(newBlock);
        break;
    }
    case Level::pipe: {
        Actor* newPipe = new Pipe(this, x, y);
        m_actors.push_back(newPipe);
    }
    }
    
}

Actor* StudentWorld::blockingBlock(Actor* curActor, int dx, int dy) 
//TODO only implemented for static objects like blocks for now
{
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        if (!(*it)->canMoveThrough() && overlap(curActor, (*it), dx, dy)) {
            return (*it);
        }
        it++;
    }
    return nullptr;
}

bool StudentWorld::collisionWithBlock(Actor* curActor, int dx, int dy)
{
    return (blockingBlock(curActor, dx, dy) != nullptr);
}

bool StudentWorld::overlapWithPeach(Actor* curActor) {
    return overlap(curActor, m_Peach);
}

bool StudentWorld::overlap(Actor* curActor, Actor* targetActor, int dx, int dy) {
    //cur Actor
    int curActorX = curActor->getX() + dx;
    int curActorY = curActor->getY() + dy;

    int tarActorX = targetActor->getX();
    int tarActorY = targetActor->getY();

    if (abs(curActorX - tarActorX) < SPRITE_WIDTH) { //both actors share a collumn
        if (abs(curActorY - tarActorY) < SPRITE_HEIGHT) return true; //both actors share a row
    }

    return false; //actors do not overlap

}

void StudentWorld::updateText() {
    ostringstream text;
    
    text << "Lives: " << getLives() << "  Level: ";
    text.fill('0');
    text.width(2);
    text << getLevel() << "  Points : ";
    text.width(6);
    text << getScore() << " ";

    if (m_Peach->hasStar()) text << "StarPower! ";
    if (m_Peach->hasShoot()) text << "ShootPower! ";
    if (m_Peach->hasJump()) text << "JumpPower! ";
    setGameStatText(text.str());

}

Peach* StudentWorld::getPeach() {
    return m_Peach;
}

//-----------Actor adders------------//
void StudentWorld::addGoodie(string type, int x, int y) {
    Actor* newGoodie = nullptr;
    if (type == MUSHROOM) newGoodie = new Mushroom(this, x, y + 8);
    else if (type == STAR) newGoodie = new Star(this, x, y + 8);
    else if (type == FLOWER) newGoodie = new Flower(this, x, y + 8);
    m_actors.push_back(newGoodie);
};