#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

float vincenty_distance(float lat1, float lon1, float lat2, float lon2, float *bearing);

#endif
