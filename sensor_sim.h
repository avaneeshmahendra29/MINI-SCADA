#ifndef SENSOR_SIM_H
#define SENSOR_SIM_H

/* Holds only the simulation's internal time step — no register or
 * networking knowledge belongs here. Swap this file out entirely when
 * you replace the simulated sensor with a real ADC/PLC reading. */
typedef struct {
    long tick;
} sensor_sim_t;

void sensor_sim_init(sensor_sim_t *sim);

/* Advances the simulation by one step and returns the new sensor value. */
int sensor_sim_next_value(sensor_sim_t *sim);

#endif /* SENSOR_SIM_H */
