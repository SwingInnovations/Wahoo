#ifndef WAHOO_STECH_CORE_H
#define WAHOO_STECH_CORE_H

#include <cmath>

#define ST_PI 3.1415926535
#define toRadian(x) (float)(((x) * ST_PI / 180.0f))
#define toDegree(x) (float)(((x) * 180.0f / ST_PI))

#ifndef USEPRECISE
#define  stReal double
#else
#define stReal float
#endif

#endif //WAHOO_STECH_CORE_H
