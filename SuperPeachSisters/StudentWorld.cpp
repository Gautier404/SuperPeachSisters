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
    m_actors = nullptr;
    m_levelCompleted = false;
    m_gameCompleted = false;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    //cerr << "making new level" << endl;
    m_actors = new std::list<Actor*>;
    m_levelCompleted = false;

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
    cerr << "made new level" << endl;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //Ask all actors to do something
    list<Actor*>::iterator it = (*m_actors).begin();
  
    while (it != (*m_actors).end()) {
        (*it)->doSomething();
        if (!m_Peach->ifAlive()) {
            playSound(SOUND_PLAYER_DIE);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        //have to add in detection for if peach dies (prolly check if she's alive)
        it++;
    }
    //if peach has reached a flag play SOUND_FINISHED_LEVEL and return GWSTATUS_FINISHED_LEVEL
    if (m_levelCompleted) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //if peach has reached mario play  SOUND_GAME_OVER and return GWSTATUS_PLAYER_WON
    if (m_gameCompleted) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }

    //delete any dead actors
    it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        if (!(*it)->ifAlive()) {
            delete (*it);
            it = (*m_actors).erase(it); //should move iterator to next value
        }
        else it++;
    }
    //update status text
    updateText();

    //if level isn't over then continue the game
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        delete (*it);
        it = (*m_actors).erase(it);
    }
    delete m_actors;
    m_actors = nullptr;
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
        (*m_actors).push_front(peachButActor);
        break;
    }
        
    case Level::block: {
        Actor* newBlock = new Block(this, x, y, NONE);
        (*m_actors).push_back(newBlock);
        break;
    }
    case Level::flower_goodie_block: {
        Actor* newBlock = new Block(this, x, y, FLOWER);
        (*m_actors).push_back(newBlock);
        break;
    }
    case Level::star_goodie_block: {
        Actor* newBlock = new Block(this, x, y, STAR);
        (*m_actors).push_back(newBlock);
        break;
    }
    case Level::mushroom_goodie_block: {
        Actor* newBlock = new Block(this, x, y, MUSHROOM);
        (*m_actors).push_back(newBlock);
        break;
    }
    case Level::pipe: {
        Actor* newPipe = new Pipe(this, x, y);
        (*m_actors).push_back(newPipe);
        break;
    }
    case Level::flag: {
        Actor* newFlag = new Flag(this, x, y);
        (*m_actors).push_back(newFlag);
        break;
    }
    case Level::mario : {
        Actor* newMario = new Mario(this, x, y);
        (*m_actors).push_back(newMario);
        break;
    }
    case Level::goomba: {
        Actor* newGoomba = new Goomba(this, x, y);
        (*m_actors).push_back(newGoomba);
        break;
    }
    case Level::koopa: {
        Actor* newKoopa = new Koopa(this, x, y);
        (*m_actors).push_back(newKoopa);
        break;
    }
    case Level::piranha: {
        Actor* newPiranha = new Piranha(this, x, y);
        (*m_actors).push_back(newPiranha);
        break;
    }
    }

    //TODO lot of repetitive code here
    
}


//----------------Overlap Handling---------------//
bool StudentWorld::bonkBlockingBlock(Actor* curActor, int dx, int dy, bool bonkDatBlock)
//TODO only implemented for static objects like blocks for now
{
    list<Actor*>::const_iterator it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        if (!(*it)->canMoveThrough() && overlap(curActor, (*it), dx, dy)) {
            if(bonkDatBlock) (*it)->bonk();
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::overlapWithPeach(Actor* curActor) const{
    return overlap(curActor, m_Peach);
}

bool StudentWorld::damageOverlap(Actor* curActor) {
    list<Actor*>::iterator it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        if ((*it)->damagable() && (*it) != m_Peach && overlap((*it), curActor) && (*it)->ifAlive()) { //TODO CHeck if not peach may be illegal
            (*it)->kill();
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::overlap(Actor* curActor, Actor* targetActor, int dx, int dy) const {
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

bool StudentWorld::isSupported(Actor* curActor, int dx) const {
    int pixelsBelow = 0;
    list<Actor*>::const_iterator it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        if (!(*it)->canMoveThrough() && curActor->getY() - (*it)->getY() == SPRITE_HEIGHT) {
            int diff = SPRITE_WIDTH - abs(curActor->getX() + dx - (*it)->getX());
            if (diff > 0) pixelsBelow += diff;
        }
        it++;
    }
    return pixelsBelow >= SPRITE_WIDTH;

};

void StudentWorld::bonkOverlapsWithPeach() {
    list<Actor*>::iterator it = (*m_actors).begin();
    while (it != (*m_actors).end()) {
        if (overlapWithPeach(*it) && (*it) != m_Peach) (*it)->bonk();
        it++;
    }
};

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

void StudentWorld::completeLevel() {
    m_levelCompleted = true;
}

void StudentWorld::completeGame() {
    m_gameCompleted = true;
}
//-----------Actor adders------------//
void StudentWorld::addGoodie(string type, int x, int y) {
    Actor* newGoodie = nullptr;
    if (type == MUSHROOM) newGoodie = new Mushroom(this, x, y + 8);
    else if (type == STAR) newGoodie = new Star(this, x, y + 8);
    else if (type == FLOWER) newGoodie = new Flower(this, x, y + 8);
    (*m_actors).push_back(newGoodie);
};

void StudentWorld::addProjectile(int type, int x, int y, int direction) {
    Actor* newProjectile = nullptr;
    if (type == PEACHFIRE) newProjectile = new PeachFireball(this, x, y, direction);
    else if (type == SHELL) newProjectile = new Shell(this, x, y, direction);
    else if (type == PIRANHAFIRE) newProjectile = new PiranhaFireball(this, x, y, direction);
    (*m_actors).push_back(newProjectile);
};


//-----------Peach interface------------//
bool StudentWorld::peachHasStar() const {
    return m_Peach->hasStar();
}

void StudentWorld::setPeachHPTo(int hp) {
    m_Peach->setHitPoint(hp);
}

void StudentWorld::givePeachPower(string power) {
    if (power == MUSHROOM) m_Peach->giveJump();
    if (power == FLOWER) m_Peach->giveShoot();
    if (power == STAR) m_Peach->giveStar();
}

void StudentWorld::hitPeach() {
    m_Peach->kill();
}

int StudentWorld::getPeachX() const{
    return m_Peach->getX();
}

int StudentWorld::getPeachY() const {
    return m_Peach->getY();
}