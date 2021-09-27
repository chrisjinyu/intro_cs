#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct Node
{
	Node(const GeoCoord& current, const GeoCoord& destination, double cost)
		:coord(current), destination(destination), g(cost)
	{
		updateH();
		f = g + h;
	}
	GeoCoord coord, destination;
	double h, g, f;

	void updateH() { h = distanceEarthMiles(coord, destination); }
};

/*OPERATORS required to properly use priority queue*/
bool operator==(const Node& n1, const Node& n2) { return n1.f == n2.f; }
bool operator!=(const Node& n1, const Node& n2) { return !(n1 == n2); }
bool operator<(const Node& n1, const Node& n2) { return n1.f > n2.f; }
bool operator>(const Node& n1, const Node& n2) { return n1.f < n2.f; }


class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	MapLoader m_ml;
	AttractionMapper m_aMap;
	SegmentMapper m_sMap;

	//Required for A* implementation
	vector<Node> neighbors(Node n) const;
	vector<Node> reconstructPath(const MyMap<GeoCoord, Node>& m, const GeoCoord gc_start, const GeoCoord gc_end) const;

	//Required to process the A* implementation into Navigator-ready components
	vector<StreetSegment> nodeToStreetList(const vector<Node>& n) const;
	const StreetSegment* findCommonStreet(const vector<StreetSegment>& s1, const vector<StreetSegment>& s2) const;
	vector<string> nodeToDirectionsList(const vector<Node>& n) const;
	vector<string> nodeToTurnList(const vector<Node>& n, const vector<StreetSegment>& s) const;
	vector<NavSegment> nodeToNavSegment(const vector<Node>& n) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (m_ml.load(mapFile))
	{
		m_aMap.init(m_ml);
		m_sMap.init(m_ml);
		return true;
	}
	return false;  // This compiles, but may not be correct
}

//Used slightly modified Pseudocode found on wikipedia
NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord gc_start, gc_end;

	//Preliminary checks
	if (!m_aMap.getGeoCoord(start, gc_start))
		return NAV_BAD_SOURCE;
	if (!m_aMap.getGeoCoord(end, gc_end))
		return NAV_BAD_DESTINATION;

	/* INITIALIZE OPEN SET (Currently considered GeoCoords) */
	priority_queue<Node> os;

	/* INITIALIZE CLOSED SET */
	MyMap<GeoCoord, double> cl_priorityMap;		//closed list must consist of two different lists, one to track closed (kind of cheating with the map, sets aren't allowed)
	MyMap<GeoCoord, Node> cl_sourceMap;			//one to track the source listings

	/* INSERT STARTING NODE */
	Node begin(gc_start, gc_end, 0.);
	os.push(begin);
	cl_sourceMap.associate(gc_start, begin);

	/* WHILE OL IS NOT EMPTY */
	while (!os.empty())
	{
		/* POP HIGHEST PRIORITY NODE FROM QUEUE */
		Node q = os.top();
		os.pop();

		/* IF THE END IS FOUND */
		if (q.coord == gc_end)
		{
			vector<Node> nodePath = reconstructPath(cl_sourceMap, gc_start, gc_end);
			directions = nodeToNavSegment(nodePath);

			return NAV_SUCCESS;
		}
		
		cl_priorityMap.associate(q.coord, q.g);		//close set

		/* GENERATE SUCCESSORS */
		vector<Node> successors = neighbors(q);

		/* FOR EACH SUCCESSOR */
		for (Node n : successors)
		{
			//used to check for existing priority value
			double* check_cl = cl_priorityMap.find(n.coord);
			//used to check for tentative priority values
			double tentative_g = q.g + distanceEarthMiles(q.coord, n.coord);

			//IF NONEXISTANT or NEW PRIORITY VALUE IS LOWER, REINSERT AND REASSOCIATE
			if(check_cl == nullptr || tentative_g < *check_cl)
			{
				n.g = tentative_g;
				n.f = n.g + n.h;

				os.push(n);
				cl_sourceMap.associate(n.coord, q);
			}
		}
	}

	return NAV_NO_ROUTE; 
}

//******************** Defined functions ************************************
vector<Node> NavigatorImpl::neighbors(Node n) const
{
	vector<StreetSegment> v_segments = m_sMap.getSegments(n.coord);
	vector<Node> successors;

	for (int i = 0; i < (int)(v_segments.size()); i++)
	{
		vector<Attraction> l_attractions = v_segments[i].attractions;			//inserts all attractions associated with segments
		for (int j = 0; j < (int)(l_attractions.size()); j++)
			if (l_attractions[j].geocoordinates == n.destination)
				successors.push_back(Node(l_attractions[j].geocoordinates, n.destination, n.g));

		if (n.coord != v_segments[i].segment.start)
			successors.push_back(Node(v_segments[i].segment.start, n.destination, n.g));	//inserts ends of segments to find neighboring nodes
		if (n.coord != v_segments[i].segment.end)
			successors.push_back(Node(v_segments[i].segment.end, n.destination, n.g));
	}

	return successors;
}

//Foslows modified pseudocode provided on wikipedia for A*
vector<Node> NavigatorImpl::reconstructPath(const MyMap<GeoCoord, Node>& m, const GeoCoord gc_start, const GeoCoord gc_end) const
{
	vector<Node> reverse_path;			//begins reverse
	Node trace = *(m.find(gc_end)), start = *(m.find(gc_start));
	reverse_path.push_back(trace);

	while (trace != start)
	{
		trace = *(m.find(trace.coord));
		reverse_path.push_back(trace);
	}

	//Needs to be added since the function originally stated only DRAWS the path
	vector<Node> path;
	for (int i = (int)(reverse_path.size()) - 1; i >= 0; i--)
	{
		path.push_back(reverse_path[i]);
	}
	return path;
}

//******************** Node Conversion Functions *******************************

vector<StreetSegment> NavigatorImpl::nodeToStreetList(const vector<Node>& n) const
{
	vector<StreetSegment> streets;

	//connect the nodes together by searching for common streets in subsequent nodes
	for (int i = 0; i < (int)(n.size())-1; i++)
	{
		vector<StreetSegment>	p_streets = m_sMap.getSegments(n[i].coord),
								r_streets = m_sMap.getSegments(n[i+1].coord);
		const StreetSegment* check = findCommonStreet(p_streets, r_streets);		//easier to compare using a pointer
		if (check != nullptr)
			streets.push_back(*check);
	}
	return streets;
}

const StreetSegment* NavigatorImpl::findCommonStreet(const vector<StreetSegment>& s1, const vector<StreetSegment>& s2) const
{
	for(int i = 0; i < (int)(s1.size());i++)
		for (int j = 0; j < (int)(s2.size());j++)
			if (s1[i].streetName == s2[j].streetName)		//same street found
				return &s1[i];
	return nullptr;
}

vector<string> NavigatorImpl::nodeToDirectionsList(const vector<Node>& n) const
{
	vector<string> directions;
	for (int i = 0; i < (int)(n.size()) - 1;i++)
	{
		GeoSegment gs(n[i].coord, n[i + 1].coord);
		double angle = angleOfLine(gs);

		//Listed directions based on angle
		if (0 <= angle && angle <= 22.5)
			directions.push_back("east");
		if (22.5 < angle && angle <= 67.5)
			directions.push_back("northeast");
		if (67.5 < angle && angle <= 112.5)
			directions.push_back("north");
		if (112.5 < angle && angle <= 157.5)
			directions.push_back("northwest");
		if (157.5 < angle && angle <= 202.5)
			directions.push_back("west");
		if (202.5 < angle && angle <= 247.5)
			directions.push_back("southwest");
		if (247.5 < angle && angle <= 292.5)
			directions.push_back("south");
		if (292.5 < angle && angle <= 337.5)
			directions.push_back("southeast");
		if (337.5 < angle && angle < 360)
			directions.push_back("east");
	}
	return directions;
}

vector<string> NavigatorImpl::nodeToTurnList(const vector<Node>& n, const vector<StreetSegment>& s) const
{
	vector<string> turns;
	for (int i = 0; i < (int)(s.size()) - 1;i++)
	{
		if (s[i].streetName != s[i + 1].streetName)	//change of street designates turn
		{
			GeoSegment gs1(n[i].coord, n[i + 1].coord), gs2(n[i + 1].coord, n[i + 2].coord);		//n will have 1 more than s almost always
			double angle = angleBetween2Lines(gs1, gs2);
			if (angle >= 180)
				turns.push_back("right");
			else
				turns.push_back("left");
		}
	}
	return turns;
}

vector<NavSegment> NavigatorImpl::nodeToNavSegment(const vector<Node>& n) const
{
	//requires processing the previous in order to create propre NavSegment objects
	vector<StreetSegment> streets = nodeToStreetList(n);
	vector<string> turns = nodeToTurnList(n, streets);
	vector<string> directions = nodeToDirectionsList(n);
	
	vector<NavSegment> navigation;

	//counters are required to run through each vector individually
	int turnCounter, directionCounter, nodeCounter;
	turnCounter = directionCounter = nodeCounter = 0;

	for (int i = 0; i < streets.size(); i++)
	{
		//Always processes a "PROCEED" command
		double distance = distanceEarthMiles(n[nodeCounter].coord, n[nodeCounter+1].coord);
		GeoSegment gs(n[nodeCounter].coord, n[nodeCounter + 1].coord);

		navigation.push_back(NavSegment(directions[directionCounter], streets[i].streetName, distance, gs));
		directionCounter++;
		nodeCounter++;

		//Only processes a "TURN" command when street is changed
		if (i != streets.size() - 1 && streets[i].streetName != streets[i + 1].streetName)
		{
			navigation.push_back(NavSegment(turns[turnCounter], streets[i + 1].streetName));
			turnCounter++;
		}
	}
	return navigation;
}
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}