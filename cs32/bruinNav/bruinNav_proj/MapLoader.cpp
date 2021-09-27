#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_segments;
};

MapLoaderImpl::MapLoaderImpl() {}

MapLoaderImpl::~MapLoaderImpl() {}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile)
		return false;

	string line;
	while (getline(infile, line))
	{
		StreetSegment seg;
		seg.streetName = line;

		//starting coordinate
		string lat, lon;
		getline(infile, lat, ',');
		while (mapFile[0] == ' ')
			getline(infile, line, ' ');
		while (lon == "") { getline(infile, lon, ' '); }
		GeoCoord start(lat, lon);

		//ending coordinate
		lat = lon = "";

		getline(infile, lat, ',');
		while (mapFile[0] == ' ')
			getline(infile, line, ' ');
		getline(infile, lon);

		GeoCoord end(lat, lon);

		GeoSegment g_seg(start, end);
		seg.segment = g_seg;

		//Attraction handling
		vector<Attraction> l_attractions;
		string s_numAttractions;
		getline(infile, s_numAttractions);
		int numAttractions = stoi(s_numAttractions);

		for (int i = 0; i < numAttractions; i++)
		{
			Attraction tempAttraction;
			string a_name;
			getline(infile, a_name, '|');

			tempAttraction.name = a_name;

			lat = lon = "";
			getline(infile, lat, ',');
			getline(infile, lon);

			tempAttraction.geocoordinates = GeoCoord(lat, lon);

			l_attractions.push_back(tempAttraction);
		}
		seg.attractions = l_attractions;
		m_segments.push_back(seg);
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return (size_t)(m_segments.size());		//hate errors
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= m_segments.size())
		return false;
	else
		seg = m_segments[segNum];		//sets seg to the segment
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
