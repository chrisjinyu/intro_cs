
#include <string>

#define TESTING

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

/*
HOMEWORK 3
Christian Yu
104785497
CS32-2 w/ Smallberg
*/

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (maze[sr][sc] == 'X')				//in case of starting at bad spot
		return false;
	if (sr == er && sc == ec)				//base case, finished
		return true;
	maze[sr][sc] = 'X';
	if (sr - 1 >= 0 && maze[sr - 1][sc] == '.')
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))			//each similar statement begins new search @ locations described
			return true;
	if (sc + 1 < nCols && maze[sr][sc + 1] == '.')
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	if (sr + 1 < nRows && maze[sr + 1][sc] == '.')
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	if (sc - 1 >= 0 && maze[sr][sc - 1] == '.')
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	return false;							//base case, all run out
}

#ifdef TESTING
#include <iostream>
int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}
#endif