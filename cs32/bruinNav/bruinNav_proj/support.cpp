#include "support.h"

bool operator==(const GeoCoord &gc1, const GeoCoord &gc2) { return gc1.latitude == gc2.latitude && gc1.longitude == gc2.longitude; }

bool operator!=(const GeoCoord &gc1, const GeoCoord &gc2) { return !(gc1 == gc2); }

bool operator <(const GeoCoord &gc1, const GeoCoord &gc2)
{
	if (gc1.latitude == gc2.latitude)
		return gc1.longitude < gc2.longitude;
	return gc1.latitude < gc2.latitude;
}

bool operator >(const GeoCoord &gc1, const GeoCoord &gc2)
{
	if (gc1.latitude == gc2.latitude)
		return gc1.longitude > gc2.longitude;
	return gc1.latitude > gc2.latitude;
}

//near copy/paste from direction specifications
std::string directionOfLine(const GeoSegment& gs)
{
	double angle = angleOfLine(gs);

	if (0 <= angle && angle <= 22.5)
		return "east";
	if (22.5 < angle && angle <= 67.5)
		return "northeast";
	if (67.5 < angle && angle <= 112.5)
		return "north";
	if (112.5 < angle && angle <= 157.5)
		return "northwest";
	if (157.5 < angle && angle <= 202.5)
		return "west";
	if (202.5 < angle && angle <= 247.5)
		return "southwest";
	if (247.5 < angle && angle <= 292.5)
		return "south";
	if (292.5 < angle && angle <= 337.5)
		return "southeast";
	if (337.5 < angle && angle < 360)
		return "east";
	return "SOME_DIRECTION";
}