#include "StudentWorld.h"
#include "Actor.h"
#include "Field.h"
#include "Compiler.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

///////////////////////////////////////////////////////
//////////////////INIT, MOVE, CLEANUP//////////////////
///////////////////////////////////////////////////////

int StudentWorld::init()
{
	//Grab file names
	vector<string> fileNames = getFilenamesOfAntPrograms();

	//errors for no bugs or too many bugs
	//if (fileNames.empty())
	//{
	//	setError("No Bugs!");
	//	return GWSTATUS_LEVEL_ERROR;
	//}
	if (fileNames.size() > MAX_ANT_COLONIES)
	{
		setError("Too Many Bugs!");
		return GWSTATUS_LEVEL_ERROR;
	}

	//set up compilers
	c0 = new Compiler, c1 = new Compiler, c2 = new Compiler, c3 = new Compiler;

	//errors for bad bug files
	string error, errorString;
	if (!fileNames.empty())
	{
		if (!c0->compile(fileNames[0], error))
		{
			errorString += fileNames[0] + " " + error + " ";
		}
		if (fileNames.size() >= 2 && !c1->compile(fileNames[1], error))
		{
			errorString += fileNames[1] + " " + error + " ";
		}
		if (fileNames.size() >= 3 && !c2->compile(fileNames[2], error))
		{
			errorString += fileNames[2] + " " + error + " ";
		}
		if (fileNames.size() == 4 && !c3->compile(fileNames[3], error))
		{
			errorString += fileNames[3] + " " + error + " ";
		}
	}
	if (errorString != "")
	{
		setError(errorString);
		return GWSTATUS_LEVEL_ERROR;
	}

	//initialize player names and count
	m_numPlayers = fileNames.size();
	m_names[0] = c0->getColonyName();
	m_names[1] = c1->getColonyName();
	m_names[2] = c2->getColonyName();
	m_names[3] = c3->getColonyName();

	//initialize ticks back to 0, read in field for use
	m_ticks = 0;
	Field f;
	string fieldFile = getFieldFilename();

	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return GWSTATUS_LEVEL_ERROR;
	}
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Field::FieldItem item = f.getContentsOf(i, j);
			switch (item)
			{
			case Field::FieldItem::rock:
				addActor(new Pebble(i, j, this));
				break;
			case Field::FieldItem::water:
				addActor(new WaterPool(i, j, this));
				break;
			case Field::FieldItem::food:
				addActor(new Food(i, j, this, false));
				break;
			case Field::FieldItem::poison:
				addActor(new Poison(i, j, this));
				break;
			case Field::FieldItem::anthill0:
				if (m_numPlayers >= 1)
					addActor(new Anthill(i, j, this, c0, 0));
				break;
			case Field::FieldItem::anthill1:
				if (m_numPlayers >= 2)
					addActor(new Anthill(i, j, this, c1, 1));
				break;
			case Field::FieldItem::anthill2:
				if (m_numPlayers >= 3)
					addActor(new Anthill(i, j, this, c2, 2));
				break;
			case Field::FieldItem::anthill3:
				if (m_numPlayers == 4)
					addActor(new Anthill(i, j, this, c3, 3));
				break;
			case Field::FieldItem::grasshopper:
				addActor(new BabyGrasshopper(i, j, this));
				break;
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//increase ticks
	m_ticks++;

	//executes all doSomethings, except TriggerableActors
	doSomethingLoop();
	//executes all triggers from TriggerableActors
	triggerLoop();
	//cleans all dead Actors
	cleanUpLoop();
	//sets the game text
	setText();

	//Game End Condition
	if (m_ticks >= 2000)
	{
		int windex = 0;
		int max = m_score[0];
		int min = m_tiebreaker[0];
		for (int i = 1; i < m_numPlayers; i++)
		{
			if (max == m_score[i] && min > m_tiebreaker[i])
			{
				min = m_tiebreaker[i];
				windex = i;
			}
			if (max < m_score[i])
			{
				max = m_score[i];
				min = m_tiebreaker[i];
				windex = i;
			}
		}
		if (max <= 5)
			return GWSTATUS_NO_WINNER;
		setWinner(m_names[windex]);
		return GWSTATUS_PLAYER_WON;
	}
	return GWSTATUS_CONTINUE_GAME;
}

//removes ALL ACTORS from the simulation
void StudentWorld::cleanUp()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator i1 = m_field[i][j].begin();
			while (i1 != m_field[i][j].end())
			{
				Actor* a = *i1;
				i1 = m_field[i][j].erase(i1);
				delete a;
			}
		}
	}
}

///////////////////////////////////////////////////////
//////////////////AUXILIARY FUNCTIONS//////////////////
///////////////////////////////////////////////////////
//described in the header file/////////////////////////
///////////////////////////////////////////////////////

bool StudentWorld::canMoveTo(int x, int y) const
{
	if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
		return false;
	for (Actor* a : m_field[x][y])
		if (a != nullptr && a->getType() == Actor::ActorType::rock)
			return false;
	return true;
}

void StudentWorld::addActor(Actor* a)
{
	m_field[a->getX()][a->getY()].push_back(a);
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
	for (Actor* a : m_field[x][y])
		if (a->getType() == Actor::ActorType::food)
			return a;
	return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
	for (Actor* a : m_field[x][y])
	{
		Pheromone* p = dynamic_cast<Pheromone*>(a);
		if (p != nullptr && p->getColony() == colony)
			return a;
	}
	return nullptr;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
	for (Actor* a : m_field[x][y])
	{
		Ant* ant = dynamic_cast<Ant*>(a);
		if (ant != nullptr)
			if (ant->getColony() != colony)
				return true;
	}
	return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
	for (Actor* a : m_field[x][y])
	{
		if (a->getType() == Actor::ActorType::poison || a->getType() == Actor::ActorType::a_grasshopper || a->getType() == Actor::ActorType::b_grasshopper)
			return true;
		Ant* ant = dynamic_cast<Ant*>(a);
		if (ant != nullptr && ant->getColony() != colony)
			return true;
	}
	return false;
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
	for (Actor* a : m_field[x][y])
	{
		Anthill* ah = dynamic_cast<Anthill*>(a);
		if (ah != nullptr && ah->getColony() == colony)
			return true;
	}
	return false;
}

bool StudentWorld::biteEnemyAt(Actor* me, int colony, int damage)
{
	vector<Insect*> targets;
	for (Actor* a : m_field[me->getX()][me->getY()])
	{
		Insect* i = dynamic_cast<Insect*>(a);
		if (i != nullptr)
			if ((i != me && colony == 4) || (i->getColony() != colony))
				targets.push_back(i);
	}
	if (targets.size() > 0)
	{
		int rand = randInt(0, targets.size() - 1);
		targets[rand]->takeDamage(damage);
		return true;
	}
	return false;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
	for (Actor *a : m_field[x][y])
	{
		Insect* i = dynamic_cast<Insect*>(a);
		if (i != nullptr && i->getType() != Actor::ActorType::a_grasshopper)
			if (!i->hasBeenPoisoned())
			{
				i->poison();
				i->EnergyActor::takeDamage(150);
			}
	}
	return true;
}

bool StudentWorld::stunAllStunnableAt(int x, int y)
{
	for (Actor *a : m_field[x][y])
	{
		Insect* i = dynamic_cast<Insect*>(a);
		if (i != nullptr && i->getType() != Actor::ActorType::a_grasshopper)
			if (!i->isWet())
				i->setWet();
	}
	return true;
}

////////////////////////////////////////////////////////////
////////////////////MAKES IT EASIER TO READ/////////////////
//ALSO ITS 6AM AND MY ROOMMATE IS SNORING/HYPERVENTILATING//
////////////////////////////////////////////////////////////

//LOOPS demonstrate clock usage, flips to and from on alternating loops

void StudentWorld::doSomethingLoop()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator i1 = m_field[i][j].begin();
			while (i1 != m_field[i][j].end())
			{
				if (!(*i1)->hasActed() && (*i1)->getType() != Actor::ActorType::poison && (*i1)->getType() != Actor::ActorType::puddle)
				{
					(*i1)->doSomething();
					(*i1)->flipClock();
				}
				if ((*i1)->getX() != i || (*i1)->getY() != j)
				{
					m_field[(*i1)->getX()][(*i1)->getY()].push_front(*i1);
					i1 = m_field[i][j].erase(i1);
				}
				else
					i1++;
			}
		}
	}
}

void StudentWorld::triggerLoop()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator i1 = m_field[i][j].begin();
			while (i1 != m_field[i][j].end())
			{
				TriggeredActor* ta = dynamic_cast<TriggeredActor*>(*i1);
				if (ta != nullptr && !ta->hasActed())
				{
					ta->doSomething();
					ta->flipClock();
				}
				i1++;
			}
		}
	}
}

void StudentWorld::cleanUpLoop()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator i1 = m_field[i][j].begin();
			while (i1 != m_field[i][j].end())
			{
				Actor* a = *i1;
				a->flipClock();
				if (a->isDead())
				{
					i1 = m_field[i][j].erase(i1);
					delete a;
				}
				else
					i1++;
			}
		}
	}
}

void StudentWorld::setText()
{
	string players;
	for (int i = 0; i < m_numPlayers; i++)
	{
		players += m_names[i] + ": " + to_string(m_score[i]) + " ants";
		if (i != m_numPlayers - 1)
			players += " | ";
	}
	setGameStatText("Ticks: " + to_string(2000 - m_ticks) + " - " + players);
}

StudentWorld::~StudentWorld()
{
	cleanUp();
	delete c0;
	delete c1;
	delete c2;
	delete c3;
}