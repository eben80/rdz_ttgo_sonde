#include "utils.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

// Vincenty's formula for distance between two points on an ellipsoid
// and the initial bearing from the first point to the second
// https://en.wikipedia.org/wiki/Vincenty%27s_formulae
float vincenty_distance(float lat1, float lon1, float lat2, float lon2, float *bearing) {
    float a = 6378137.0; // WGS-84 ellipsoid major axis
    float f = 1.0 / 298.257223563; // WGS-84 ellipsoid flattening
    float b = (1.0 - f) * a; // WGS-84 ellipsoid minor axis

    float L = (lon2 - lon1) * (PI / 180.0);
    float U1 = atan((1.0 - f) * tan(lat1 * (PI / 180.0)));
    float U2 = atan((1.0 - f) * tan(lat2 * (PI / 180.0)));
    float sinU1 = sin(U1);
    float cosU1 = cos(U1);
    float sinU2 = sin(U2);
    float cosU2 = cos(U2);

    float lambda = L;
    float lambdaP = 2.0 * PI;
    int iterLimit = 100;
    float sinLambda, cosLambda, sinSigma, cosSigma, sigma, sinAlpha, cosSqAlpha, cos2SigmaM, C;

    while (abs(lambda - lambdaP) > 1e-12 && --iterLimit > 0) {
        sinLambda = sin(lambda);
        cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2 * sinLambda) * (cosU2 * sinLambda) + (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda) * (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda));
        if (sinSigma == 0) return 0; // co-incident points
        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SigmaM = cosSigma - 2.0 * sinU1 * sinU2 / cosSqAlpha;
        if (isnan(cos2SigmaM)) cos2SigmaM = 0; // equatorial line
        C = f / 16.0 * cosSqAlpha * (4.0 + f * (4.0 - 3.0 * cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1.0 - C) * f * sinAlpha * (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM)));
    }

    if (iterLimit == 0) return NAN; // formula failed to converge

    float uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    float A = 1.0 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
    float B = uSq / 1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq)));
    float deltaSigma = B * sinSigma * (cos2SigmaM + B / 4.0 * (cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM) - B / 6.0 * cos2SigmaM * (-3.0 + 4.0 * sinSigma * sinSigma) * (-3.0 + 4.0 * cos2SigmaM * cos2SigmaM)));

    float s = b * A * (sigma - deltaSigma);

    if (bearing) {
        float fwdAz = atan2(cosU2 * sinLambda, cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
        *bearing = fwdAz * (180.0 / PI);
        if (*bearing < 0) *bearing += 360.0;
    }

    return s;
}
