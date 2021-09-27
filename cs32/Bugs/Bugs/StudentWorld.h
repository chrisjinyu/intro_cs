#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//required for pointers
class Actor;
class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_ticks(0) {}

	virtual ~StudentWorld();

	//Three important functions for operation
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// Can an insect move to x,y?
	bool canMoveTo(int x, int y) const;

	// Add an actor to the world
	void addActor(Actor* a);

	// If an item that can be picked up to be eaten is at x,y, return a
	// pointer to it; otherwise, return a null pointer.  (Edible items are
	// only ever going be food.)
	Actor* getEdibleAt(int x, int y) const;

	// If a pheromone of the indicated colony is at x,y, return a pointer
	// to it; otherwise, return a null pointer.
	Actor* getPheromoneAt(int x, int y, int colony) const;

	// Is an enemy of an ant of the indicated colony at x,y?
	bool isEnemyAt(int x, int y, int colony) const;

	// Is something dangerous to an ant of the indicated colony at x,y?
	bool isDangerAt(int x, int y, int colony) const;

	// Is the anthill of the indicated colony at x,y?
	bool isAntHillAt(int x, int y, int colony) const;

	// Bite an enemy of an ant of the indicated colony at me's location
	// (other than me; insects don't bite themselves).  Return true if an
	// enemy was bitten.
	bool biteEnemyAt(Actor* me, int colony, int biteDamage);

	// Poison all poisonable actors at x,y.
	bool poisonAllPoisonableAt(int x, int y);

	// Stun all stunnable actors at x,y.
	bool stunAllStunnableAt(int x, int y);

	// Record another ant birth for the indicated colony, records the time at which it is achieved
	void increaseScore(int colony) { m_score[colony]++; m_tiebreaker[colony] = m_ticks; }

private:
	//compilers
	Compiler *c0, *c1, *c2, *c3;

	//actor structure, not the very very bestest
	std::list<Actor*> m_field[VIEW_WIDTH][VIEW_HEIGHT];

	//program run timer
	int m_ticks;

	//Number of active players
	int m_numPlayers;

	//used to determine winner
	int m_score[MAX_ANT_COLONIES];
	int m_tiebreaker[MAX_ANT_COLONIES];
	std::string m_names[MAX_ANT_COLONIES];

	//bogged down in code
	void doSomethingLoop();
	void triggerLoop();
	void cleanUpLoop();
	void setText();
};

#endif // STUDENTWORLD_H_
