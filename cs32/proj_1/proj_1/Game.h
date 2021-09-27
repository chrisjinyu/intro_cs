#ifndef GAME_H				//guard
#define GAME_H

#include <string>			//required to create string class function

class Arena;				//incomplete class declaration  required to refer to functions

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	std::string takePlayerTurn();
};

// These functions are only used by the Game class
bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);

#endif