#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"

//Comparison operators required to use maps, which are comparative binary search trees
//comparisons do not matter too much as the GeoCoord simply needs to be split along some binary criteria
//to fit the BST
bool operator ==(const GeoCoord &gc1, const GeoCoord &gc2);
bool operator !=(const GeoCoord &gc1, const GeoCoord &gc2);
bool operator <(const GeoCoord &gc1, const GeoCoord &gc2);
bool operator >(const GeoCoord &gc1, const GeoCoord &gc2);

std::string directionOfLine(const GeoSegment& gs);

#endif