#include "utils.h"
#include <math.h>

#ifndef PI
#define  PI  (3.1415926535897932384626433832795)
#endif

#define EARTH_RADIUS 6371000.0

float calcLatLonDist(float lat1, float lon1, float lat2, float lon2) {
  float dLat = (lat2 - lat1) * PI / 180.0;
  float dLon = (lon2 - lon1) * PI / 180.0;
  float a = sin(dLat / 2) * sin(dLat / 2) +
            cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) *
            sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return EARTH_RADIUS * c;
}