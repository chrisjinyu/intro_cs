#ifndef MAZE
#define MAZE

#include <string>
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

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

#endif