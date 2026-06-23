#include "alarm.h"

int alarm_evaluate(int sensor_value, int threshold)
{
    return (sensor_value > threshold) ? 1 : 0;
}
