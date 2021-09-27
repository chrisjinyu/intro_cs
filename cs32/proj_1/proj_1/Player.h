#ifndef PLAYER_H			//guard
#define PLAYER_H

#include <string>			//required to create string functions

class Arena;				//required to create object member of Arena type without constructor

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	std::string dropPoisonPellet();
	std::string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

#endif		//end guard
