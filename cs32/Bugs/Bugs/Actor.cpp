#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"

///////////////////////////////////////////////////////
////////////////ALL THE doSomething()s/////////////////
///////////////////////////////////////////////////////

void Anthill::doSomething()
{
	if (takeDamage(1))			//tick down, checks for death
		return;
	if (consume(10000))			//eat up to 10k food
		return;
	if (getEnergy() >= 2000)	//produce an ant if enough HP
	{
		produceAnt();
	}
}

void WaterPool::doSomething()
{
	getWorld()->stunAllStunnableAt(getX(), getY());
}

void Poison::doSomething()
{
	getWorld()->poisonAllPoisonableAt(getX(), getY());
}

void BabyGrasshopper::doSomething()
{
	if (!tickCheck()) return;		//ticks, checks for death
	if (getEnergy() >= 1600)		//grow if got swole
	{
		grow();
		return;
	}
	if (!eat())		//50% chance to return true
		walk();		//walks one movement, ticks down one remaining
	setStuns(2);	//pause 2 tick
}

void AdultGrasshopper::doSomething()
{
	if (!tickCheck())				//ticks, checks for death
		return;
	if (!attack())					//attack first, 33%
		if (!jump())				//jump, 10%
			if (!eat())				//eat, 50% if eaten
				walk();				//attempts to walk if no other action taken
	setStuns(2);					//tick 2
}

void Ant::doSomething()
{
	if (!tickCheck())				//ticks, checks for death
		return;

	Compiler::Command cmd;			//oh boy

	for (int i = 0; i < 10; i++)
	{
		if (!m_compiler->getCommand(m_instruction, cmd))		//false if line error, kills ant
		{
			Actor::setDead();
			return;
		}
		m_instruction++;
		switch (cmd.opcode)					//depending on opcode
		{
		case Compiler::Opcode::moveForward:	//moveForward one space
			if (moveForward())
			{
				m_hasBeenBitten = false;		//successful movement
				m_wasBlocked = false;
			}
			else
				m_wasBlocked = true;			//unsuccessful
			return;
		case Compiler::Opcode::eatFood:
			int min;							//basically stripped down consume
			if (m_heldFood < 100)
				min = m_heldFood;
			else
				min = 100;
			addEnergy(min);
			m_heldFood -= min;
			return;
		case Compiler::Opcode::dropFood:
			dropFood();							//drops food
			return;
		case Compiler::Opcode::bite:
			bite(15);							//damage 15 random target
			return;
		case Compiler::Opcode::pickupFood:
			pickupFood();						//pick up food, add to counter
			return;
		case Compiler::Opcode::emitPheromone:
			emitPheromone();					//drops pheromone
			return;
		case Compiler::Opcode::faceRandomDirection:
			setRandDirection();					//random direction
			return;
		case Compiler::Opcode::rotateClockwise:
			turnRight();
			return;
		case Compiler::Opcode::rotateCounterClockwise:
			turnLeft();
			return;

		case Compiler::Opcode::generateRandomNumber:
			m_lastRandNum = randInt(0, std::stoi(cmd.operand1));		//random integer, usually for chance based
			break;
		case Compiler::Opcode::goto_command:
			m_instruction = stoi(cmd.operand1);		//sends to line
			break;
		case Compiler::Opcode::if_command:
			int c = stoi(cmd.operand1);				//sends to line if condition is met
			if (condition(c))
				m_instruction = stoi(cmd.operand2);
			break;
		}
	}
}

///////////////////////////////////////////////////////
//////////////////AUXILIARY FUNCTIONS//////////////////
///////////////////////////////////////////////////////

//Takes the specified damage, then sets to dead if threshhold reached
bool EnergyActor::takeDamage(int damage)
{
	decrementEnergy(damage);
	if (checkDead())
	{
		setDead();
		return true;
	}
	return false;
}

//Eats food on current spot, if available
bool EnergyActor::consume(int amount)
{
	Food* f = dynamic_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (f != nullptr)
	{
		int min;
		if (f->getEnergy() < amount)
			min = f->getEnergy();
		else
			min = amount;
		addEnergy(min);
		f->takeDamage(min);
		return true;
	}
	return false;
}

//Creates a new ant with specified members
void Anthill::produceAnt()
{
	if (m_colony == 0)
		getWorld()->addActor(new Ant(IID_ANT_TYPE0, getX(), getY(), getWorld(), getCompiler(), 0));
	if (m_colony == 1)
		getWorld()->addActor(new Ant(IID_ANT_TYPE1, getX(), getY(), getWorld(), getCompiler(), 1));
	if (m_colony == 2)
		getWorld()->addActor(new Ant(IID_ANT_TYPE2, getX(), getY(), getWorld(), getCompiler(), 2));
	if (m_colony == 3)
		getWorld()->addActor(new Ant(IID_ANT_TYPE3, getX(), getY(), getWorld(), getCompiler(), 3));
	decrementEnergy(1500);
	getWorld()->increaseScore(m_colony);
}

//random direction
void Insect::setRandDirection()
{
	int rand = randInt(0, 3);
	switch (rand)
	{
	case 0:
		setDirection(right);
		break;
	case 1:
		setDirection(down);
		break;
	case 2:
		setDirection(left);
		break;
	case 3:
		setDirection(up);
		break;
	default:
		setDirection(none);
	}
}

//Ticks down by one, then checks for death and stun conditions.
bool Insect::tickCheck()
{
	if (Insect::takeDamage(1))
		return false;
	if (getStuns() > 0)
	{
		tickStuns();
		return false;
	}
	dry();			//enables restun
	antidote();		//enables repoison
	return true;
}

//Sets dead, then adds in a food object or adds to current food energy
void Insect::setDead()
{
	EnergyActor::setDead();
	Food* f = dynamic_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (f != nullptr)
		f->addEnergy(100);
	else
		getWorld()->addActor(new Food(getX(), getY(), getWorld(), true));
}

//checks for bite
bool Insect::bite(int damage)
{
	return getWorld()->biteEnemyAt(this, getColony(), damage);
}

//moves forward in given direction, if possible
bool Insect::moveForward()
{
	Direction dir = getDirection();
	switch (dir)
	{
	case left:
		if (getWorld()->canMoveTo(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		break;
	case up:
		if (getWorld()->canMoveTo(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		break;
	case right:
		if (getWorld()->canMoveTo(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		break;
	case down:
		if (getWorld()->canMoveTo(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		break;
	}
	return false;
}

///////////////////////////////////////////////////////
/////////////////////ANT FUNCTIONS/////////////////////
///////////////////////////////////////////////////////

//drops food in the word, or adds it
void Ant::dropFood()
{
	Food* f = dynamic_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (f != nullptr)
		f->addEnergy(m_heldFood);
	else
	{
		f = new Food(getX(), getY(), getWorld(), true);
		f->setEnergy(m_heldFood);
		getWorld()->addActor(f);
	}
	m_heldFood = 0;
}

//removes up to 400 food from the square, adds to held count, maximum 1800
void Ant::pickupFood()
{
	Food* f = dynamic_cast<Food*>(getWorld()->getEdibleAt(getX(), getY()));
	if (f != nullptr)
	{
		int min;
		if (f->getEnergy() < 400)
			min = f->getEnergy();
		else
			min = 400;
		if (m_heldFood + min > 1800)
			min = 1800 - m_heldFood;
		m_heldFood += min;
		f->takeDamage(min);
	}
}

//drops pheromone of specified colony
void Ant::emitPheromone()
{
	Pheromone* p = dynamic_cast<Pheromone*>(getWorld()->getPheromoneAt(getX(), getY(), getColony()));
	if (p != nullptr)
	{
		p->addEnergy(256);
		if (p->getEnergy() > 768)
			p->setEnergy(768);
	}
	else
	{
		if (getColony() == 0)
			getWorld()->addActor(new Pheromone(IID_PHEROMONE_TYPE0, getX(), getY(), getWorld(), 0));
		if (getColony() == 1)
			getWorld()->addActor(new Pheromone(IID_PHEROMONE_TYPE1, getX(), getY(), getWorld(), 1));
		if (getColony() == 2)
			getWorld()->addActor(new Pheromone(IID_PHEROMONE_TYPE2, getX(), getY(), getWorld(), 2));
		if (getColony() == 3)
			getWorld()->addActor(new Pheromone(IID_PHEROMONE_TYPE3, getX(), getY(), getWorld(), 3));
	}
}

//processes the various conditions using data already known, all functions understandable by name
bool Ant::condition(int o)
{
	if (o == 0 || o == 1)		//both use directional checks
	{
		int x = getX();
		int y = getY();
		Direction dir = getDirection();
		if (dir == left)
			x--;
		else if (dir == up)
			y++;
		else if (dir == right)
			x++;
		else if (dir == down)
			y--;
		if (o == 0 && getWorld()->isDangerAt(x, y, getColony()))
			return true;
		else if (o == 1 && getWorld()->getPheromoneAt(x, y, getColony()) != nullptr)
			return true;
	}
	if (o == 2)
		return m_hasBeenBitten;
	if (o == 3)
		return m_heldFood > 0;
	if (o == 4)
		return getEnergy() <= 25;
	if (o == 5)
		return getWorld()->isAntHillAt(getX(), getY(), getColony());
	if (o == 6)
		return getWorld()->getEdibleAt(getX(), getY()) != nullptr;
	if (o == 7)
		return getWorld()->isEnemyAt(getX(), getY(), getColony());
	if (o == 8)
		return m_wasBlocked;
	if (o == 9)
		return m_lastRandNum == 0;
	return false;
}

void Ant::turnLeft()
{
	Direction dir = getDirection();
	if (dir == left)
		setDirection(down);
	else if (dir == up)
		setDirection(left);
	else if (dir == right)
		setDirection(up);
	else if (dir == down)
		setDirection(right);
}

void Ant::turnRight()
{
	Direction dir = getDirection();
	if (dir == left)
		setDirection(up);
	else if (dir == up)
		setDirection(right);
	else if (dir == right)
		setDirection(down);
	else if (dir == down)
		setDirection(left);
}

///////////////////////////////////////////////////////
/////////////////GRASSHOPPER FUNCTIONS/////////////////
///////////////////////////////////////////////////////

//walks forward one, or resets
void Grasshopper::walk()
{
	if (getWalk() <= 0)
		resetWalk();
	if (!moveForward())
		setWalk(0);
	setWalk(getWalk() - 1);
}

//attempts to eat on square
bool Grasshopper::eat()
{
	if (consume(200))
		if (randInt(0, 1))
			return true;
	return false;
}

//adds an Adult Grasshopper in its place, dies
void BabyGrasshopper::grow()
{
	getWorld()->addActor(new AdultGrasshopper(getX(), getY(), getWorld()));
	setDead();
}

//teleportation
bool AdultGrasshopper::jump()
{
	int rand = randInt(0, 9);
	if (rand == 0)
	{
		int radius = 0, angle = 0;
		double radians;
		int castX = VIEW_WIDTH, castY = VIEW_HEIGHT;		//potential deltaX and deltaY
		for (int i = 0; i < 100 && !getWorld()->canMoveTo(getX() + castX, getY() + castY); i++)		//attempt hundred times, just in case it fails (statistically, it should be very close to the same)
		{
			radius = randInt(1, 10);
			angle = randInt(0, 359);
			radians = angle*3.1415926 / 180.;
			castX = (int)(radius*cos(radians));
			castY = (int)(radius*sin(radians));				//run of the mill angle calculations
		}
		if (getWorld()->canMoveTo(getX() + castX, getY() + castY))
		{
			moveTo(getX() + castX, getY() + castY);			//movement
			return true;
		}
	}
	return false;
}

//modified attack method based on probability
bool AdultGrasshopper::attack()
{
	int rand = randInt(0, 2);
	if (rand == 0 && bite(50))
		return true;
	return false;
}

//retaliation overload method
bool AdultGrasshopper::takeDamage(int damage)
{
	if (Grasshopper::takeDamage(damage))
		return true;
	if (randInt(0, 1))
		attack();
	return false;
}

///////////////////////////////////////////////////////
//////////////////////CONSTRUCTORS/////////////////////
///////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, int depth = 0)
	:GraphObject(imageID, startX, startY, right, depth), m_world(world)
{
	//assign enumeration based on image
	switch (imageID)
	{
	case IID_ROCK:
		m_type = rock;
		break;
	case IID_WATER_POOL:
		m_type = puddle;
		break;
	case IID_PHEROMONE_TYPE0:
		m_type = pheromone0;
		break;
	case IID_PHEROMONE_TYPE1:
		m_type = pheromone1;
		break;
	case IID_PHEROMONE_TYPE2:
		m_type = pheromone2;
		break;
	case IID_PHEROMONE_TYPE3:
		m_type = pheromone3;
		break;
	case IID_FOOD:
		m_type = food;
		break;
	case IID_POISON:
		m_type = poison;
		break;
	case IID_BABY_GRASSHOPPER:
		m_type = b_grasshopper;
		break;
	case IID_ADULT_GRASSHOPPER:
		m_type = a_grasshopper;
		break;
	case IID_ANT_TYPE0:
		m_type = ant0;
		break;
	case IID_ANT_TYPE1:
		m_type = ant1;
		break;
	case IID_ANT_TYPE2:
		m_type = ant2;
		break;
	case IID_ANT_TYPE3:
		m_type = ant3;
		break;
	case IID_ANT_HILL:
		m_type = anthill;
		break;
	}
}
Pebble::Pebble(int startX, int startY, StudentWorld* world)
	:Actor(IID_ROCK, startX, startY, world, 1) { }
EnergyActor::EnergyActor(int imageID, int startX, int startY, int energy, StudentWorld* world, int depth = 0)
	: Actor(imageID, startX, startY, world, depth), m_energy(energy) {}
Pheromone::Pheromone(int imageID, int startX, int startY, StudentWorld* world, int colony)
	: EnergyActor(imageID, startX, startY, 256, world, 2), m_colony(colony) {}
Food::Food(int startX, int startY, StudentWorld* world, bool fromDead)
	: EnergyActor(IID_FOOD, startX, startY, 0, world, 2)
{
	if (fromDead)
		setEnergy(100);
	else
		setEnergy(6000);
}
Anthill::Anthill(int startX, int startY, StudentWorld* world, Compiler* compiler, int colony)
	:EnergyActor(IID_ANT_HILL, startX, startY, 8999, world, 2), m_compiler(compiler), m_colony(colony) {}
TriggeredActor::TriggeredActor(int imageID, int startX, int startY, StudentWorld* world)
	: Actor(imageID, startX, startY, world, 2) {}
WaterPool::WaterPool(int startX, int startY, StudentWorld* world)
	: TriggeredActor(IID_WATER_POOL, startX, startY, world) {}
Poison::Poison(int startX, int startY, StudentWorld* world)
	: TriggeredActor(IID_POISON, startX, startY, world) {}
Insect::Insect(int imageID, int startX, int startY, int health, StudentWorld* world, int colony)
	: EnergyActor(imageID, startX, startY, health, world, 1), m_colony(colony) {}
Ant::Ant(int imageID, int startX, int startY, StudentWorld* world, Compiler* compiler, int colony)
	: Insect(imageID, startX, startY, 1500, world, colony), m_compiler(compiler) {}
Grasshopper::Grasshopper(int imageID, int startX, int startY, int health, StudentWorld* world)
	: Insect(imageID, startX, startY, health, world, 4) {
	resetWalk();
}
BabyGrasshopper::BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* world)
	: Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, 500, world) {}
AdultGrasshopper::AdultGrasshopper::AdultGrasshopper(int startX, int startY, StudentWorld* world)
	: Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, 1600, world) {}