#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

///////////////////////////////////////////////////////
////////////////ACTOR CLASS DECLARATION////////////////
///////////////////////////////////////////////////////

class Actor : public GraphObject
{
public:
	//Enumeration used to avoid excessive use of dynamic_cast in identifying Actors
	enum ActorType
	{
		rock, food, pheromone0, pheromone1, pheromone2, pheromone3, puddle, poison,
		b_grasshopper, a_grasshopper,
		ant0, ant1, ant2, ant3,
		anthill
	};

	Actor(int imageID, int startX, int startY, StudentWorld* world, int depth);

	virtual ~Actor() {}

	StudentWorld* getWorld() const { return m_world; }

	ActorType getType() const { return m_type; }

	virtual void doSomething() = 0;

	//Acting clocks keep track of actors who have and have not acted
	bool hasActed() const { return m_actClock; }
	void flipClock() { m_actClock = !m_actClock; }

	//All actors must have a "dead" state where they can be cleaned up
	bool isDead() const { return m_isDead; }
	virtual void setDead() { m_isDead = true; }

private:
	//enumeration
	ActorType m_type;

	//pointer to world it exists in so it may interact with other actors
	StudentWorld* m_world;

	//acting clocks
	bool m_actClock = false;
	bool m_isDead = false;
};

///////////////////////////////////////////////////////
////////////////PEBBLE CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class Pebble : public Actor
{
public:
	//self explanatory
	Pebble(int startX, int startY, StudentWorld* world);
	virtual void doSomething() {}
};

///////////////////////////////////////////////////////
///////////////E_ACTOR CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class EnergyActor : public Actor
{
public:
	EnergyActor(int imageID, int startX, int startY, int energy, StudentWorld* world, int depth);

	//ACCESSOR
	int getEnergy() const { return m_energy; }
	bool checkDead() const { return m_energy <= 0; }

	//MODIFIERS
	void decrementEnergy() { m_energy--; }
	void decrementEnergy(int dec) { m_energy -= dec; }
	void addEnergy(int add) { m_energy += add; }
	void setEnergy(int energy) { m_energy = energy; }

	//check for death on damage
	virtual bool takeDamage(int damage);

	//consume food at location and add to energy
	bool consume(int amount);

private:
	//only new addition
	int m_energy;
};

///////////////////////////////////////////////////////
///////////////PHEROMONE CLASS DECLARATION/////////////
///////////////////////////////////////////////////////

class Pheromone : public EnergyActor
{
public:
	Pheromone(int imageID, int startX, int startY, StudentWorld* world, int colony);
	virtual void doSomething() { takeDamage(1); }

	//ACCESSOR
	int getColony() const { return m_colony; }

private:
	int m_colony;
};

///////////////////////////////////////////////////////
/////////////////FOOD CLASS DECLARATION////////////////
///////////////////////////////////////////////////////

class Food : public EnergyActor
{
public:
	//Takes in boolean to see if it is a carcass or not
	Food(int startX, int startY, StudentWorld* world, bool fromDead);
	virtual void doSomething() {}
};

///////////////////////////////////////////////////////
///////////////A_HILL CLASS DECLARATION////////////////
///////////////////////////////////////////////////////

class Anthill : public EnergyActor
{
public:
	Anthill(int startX, int startY, StudentWorld* world, Compiler* compiler, int colony);
	virtual void doSomething();

	//ACCESSORS
	int getColony() const { return m_colony; }
	Compiler* getCompiler() const { return m_compiler; }

private:
	Compiler* m_compiler;
	int m_colony;

	//produces an ant, decreases energy
	void produceAnt();
};

///////////////////////////////////////////////////////
///////////////T_ACTOR CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class TriggeredActor : public Actor
{
public:
	//only used for casting purposes in triggering Actors
	TriggeredActor(int imageID, int startX, int startY, StudentWorld* world);
};

///////////////////////////////////////////////////////
////////////////PUDDLE CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class WaterPool : public TriggeredActor
{
public:
	WaterPool(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
};

///////////////////////////////////////////////////////
////////////////POISON CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class Poison : public TriggeredActor
{
public:
	Poison(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
};

///////////////////////////////////////////////////////
////////////////INSECT CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class Insect : public EnergyActor
{
public:
	Insect(int imageID, int startX, int startY, int health, StudentWorld* world, int colony);

	//ACCESSORS
	int getStuns() const { return m_stunTicks; }
	int getColony() const { return m_colony; }
	bool isWet() const { return m_wet; }
	bool hasBeenPoisoned() const { return m_poisoned; }

	//MODIFIERS
	void setStuns(int ticks) { m_stunTicks = ticks; }
	void tickStuns() { m_stunTicks--; }
	void setWet() { m_stunTicks += 2; m_wet = true; m_lastWetX = getX(); m_lastWetY = getY(); }
	void dry() { if (getX() != m_lastWetX || getY() != m_lastWetY) m_wet = false; }
	void poison() { m_poisoned = true; m_lastPoisonX = getX(); m_lastPoisonY = getY(); }
	void antidote() { if (getX() != m_lastPoisonX || getY() != m_lastPoisonY) m_poisoned = false; }

protected:
	bool tickCheck();				//decrements health by 1, checks death state, stun state
	virtual void setDead();			//new setDead, drops food on death
	virtual bool bite(int damage);	//bite function added
	void setRandDirection();		//random direction common in all insects
	bool moveForward();				//returns true if successful

private:
	int m_colony;					//IDs insects based on where they come from
	int m_stunTicks;

	//Trigger Helper
	bool m_wet = false;
	int m_lastWetX = -1, m_lastWetY = -1;
	bool m_poisoned = false;
	int m_lastPoisonX = -1, m_lastPoisonY = -1;
};

///////////////////////////////////////////////////////
/////////////////ANT CLASS DECLARATION/////////////////
///////////////////////////////////////////////////////

class Ant : public Insect
{
public:
	Ant(int imageID, int startX, int startY, StudentWorld* world, Compiler* compiler, int colony);
	virtual void doSomething();

private:
	Compiler* m_compiler;
	int m_instruction;
	int m_counter;
	int m_heldFood;

	//ant memory
	bool m_hasBeenBitten;
	bool m_wasBlocked;
	int m_lastRandNum = 0;

	//ant helper, self-explanatory
	void dropFood();
	void pickupFood();
	void emitPheromone();

	void turnRight();
	void turnLeft();
	//process conditionals
	bool condition(int o);
};

///////////////////////////////////////////////////////
///////////////GHOPPER CLASS DECLARATION///////////////
///////////////////////////////////////////////////////

class Grasshopper : public Insect
{
public:
	Grasshopper(int imageID, int startX, int startY, int health, StudentWorld* world);

	//The following functions never have to be called outside of Grasshopper objects
protected:
	int getWalk() const { return m_walkAmt; }
	void setWalk(int walk) { m_walkAmt = walk; }
	void walk();
	//returns 50/50 t/f if eating food, else false
	bool eat();

private:
	int m_walkAmt;

	void resetWalk()
	{
		setRandDirection();
		setWalk(randInt(2, 10));
	}
};

///////////////////////////////////////////////////////
//////////////GRASSYBAE CLASS DECLARATION//////////////
///////////////////////////////////////////////////////

class BabyGrasshopper : public Grasshopper
{
public:
	BabyGrasshopper(int startX, int startY, StudentWorld* world);
	virtual void doSomething();

private:
	//git big
	void grow();
};

///////////////////////////////////////////////////////
////////////////ADULT CLASS DECLARATION////////////////
///////////////////////////////////////////////////////

class AdultGrasshopper : public Grasshopper
{
public:
	AdultGrasshopper(int startX, int startY, StudentWorld* world);
	virtual void doSomething();

	//modified takeDamage for retaliation
	virtual bool takeDamage(int damage);

private:
	//snazzy new movements
	bool jump();
	bool attack();
};
#endif // ACTOR_H_
