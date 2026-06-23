#include <math.h>
#include "sensor_sim.h"
#include "config.h"

void sensor_sim_init(sensor_sim_t *sim)
{
    sim->tick = 0;
}

int sensor_sim_next_value(sensor_sim_t *sim)
{
    sim->tick++;
    return (int)(SENSOR_CENTER + SENSOR_AMPLITUDE * sin(sim->tick * SENSOR_ANGULAR_STEP));
}
