#ifndef GAME_H
#define GAME_H

#include "Arena.h"
#include "Player.h"
#include "globals.h"
#include <string>
#include <iostream>
using namespace std;

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
	string takePlayerTurn();
};

bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);

#endif