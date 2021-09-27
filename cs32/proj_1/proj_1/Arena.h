#ifndef ARENA_H				//guard
#define ARENA_H

/* INCLUDES */
#include "globals.h"		//required to construct a grid of maximum constants MAXROWS, MAXCOLS
#include "History.h"		//required to construct a History object without calling the invalid default constructor
#include <string>			//required to use strings in prototypes of functions

/* INCOMPLETE CLASS DECLARATIONS */
// required to refer to pointers to objects of each class
class Player;
class Rat;


class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     ratCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfRatsAt(int r, int c) const;
	void    display(std::string msg) const;

	History& history();				//new feature: history, accesses the Arena object's History object by reference

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addRat(int r, int c);
	bool addPlayer(int r, int c);
	void moveRats();

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Rat*    m_rats[MAXRATS];
	int     m_nRats;
	int     m_turns;
	History m_history = History(MAXROWS,MAXCOLS);	//includes a History object with constructed grid of maximum

	// Helper functions
	void checkPos(int r, int c) const;
	bool isPosInBounds(int r, int c) const;
};

#endif	//end guard