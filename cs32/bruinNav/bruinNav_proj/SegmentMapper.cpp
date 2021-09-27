#include "provided.h"
#include <vector>

#include "MyMap.h"
#include "support.h"
using namespace std;

bool operator==(const StreetSegment &ss1, const StreetSegment &ss2)
{
	return ss1.streetName == ss2.streetName
		&& ss1.segment.start == ss2.segment.start && ss2.segment.end == ss2.segment.end;
}

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap <GeoCoord, vector<StreetSegment>> m_map;
	void insert(vector<StreetSegment>* check, StreetSegment& seg, GeoCoord& gc);
};

SegmentMapperImpl::SegmentMapperImpl() {}

SegmentMapperImpl::~SegmentMapperImpl() {}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < (int)(ml.getNumSegments()); i++)			//hate warnings
	{
		StreetSegment seg;
		ml.getSegment(i, seg);
		
		GeoCoord gc_start = seg.segment.start, gc_end = seg.segment.end;		//find segment ends in order to complete mapping

		vector<StreetSegment>* p_segCheck = m_map.find(gc_start);			//insert ends
		insert(p_segCheck, seg, gc_start);							//insertion call checks for non existing
		
		p_segCheck = m_map.find(gc_end);
		insert(p_segCheck, seg, gc_end);

		vector<Attraction> l_attractions = seg.attractions;					//insert attractions
		for (int j = 0; j < (int)(l_attractions.size()); j++)
		{
			GeoCoord gc_attraction = l_attractions[j].geocoordinates;
			p_segCheck = m_map.find(gc_attraction);
			insert(p_segCheck, seg, gc_attraction);
		}

	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment>* segments;
	segments = m_map.find(gc);
	if (segments == nullptr)
		return vector<StreetSegment>();
	return *segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}

//******************** Defined Functions ********************

void SegmentMapperImpl::insert(vector<StreetSegment>* check, StreetSegment& seg, GeoCoord& gc)
{
	if (check == nullptr)						//checks to see if the pointer exists
	{
		vector<StreetSegment> add_segmentArray;
		add_segmentArray.push_back(seg);
		m_map.associate(gc, add_segmentArray);
	}
	else
	{
		bool c_contains = false;
		for (int j = 0; j < (int)(check->size());j++)
			if ((*check)[j] == seg)
				c_contains = true;
		if (!c_contains)
			check->push_back(seg);
	}
}