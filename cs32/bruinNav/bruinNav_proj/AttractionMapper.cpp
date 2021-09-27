#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;

private:
	MyMap<string, GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl() {}

AttractionMapperImpl::~AttractionMapperImpl() {}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < (int)(ml.getNumSegments()); i++)		//I hate warnings
	{
		StreetSegment seg;
		ml.getSegment(i, seg);

		for (int j = 0; j < (int)(seg.attractions.size()); j++)
		{
			string a_name = seg.attractions[j].name;
			//CASE INSENSITIVE
			for (int k = 0; k < (int)(a_name.size()); k++)
				a_name[k] = tolower(a_name[k]);
			m_map.associate(a_name, seg.attractions[j].geocoordinates);		//associates name to the corresponding coordinate
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	//CASE INSENSITIVE SEEEEAARCH
	for (int k = 0; k < (int)(attraction.size()); k++)
		attraction[k] = tolower(attraction[k]);

	const GeoCoord* g_coord = m_map.find(attraction);
	if (g_coord == nullptr)
		return false;

	gc = *g_coord;		//sets gc to the corresponding found coordinate
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}