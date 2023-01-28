#include <math.h>
 
double wind_chill(int wind_speed, int temp)
{
      return (((10.45 + (6.686112 * sqrt((double) wind_speed))
            - (.447041 * wind_speed)) / 22.034 * (temp - 91.4)) + 91.4);
}
