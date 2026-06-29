#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "config.h"
#include "register_store.h"
#include "sensor_sim.h"
#include "alarm.h"
#include "tcp_server.h"

/*
 * The simulation loop is the one place that ties sensor_sim, alarm, and
 * register_store together. Each module on its own doesn't know about the
 * others — this function is the "wiring".
 */
static void *simulation_loop(void *arg)
{
    register_store_t *store = (register_store_t *)arg;

    sensor_sim_t sim;
    sensor_sim_init(&sim);

    while (1) {
        int sensor_value = sensor_sim_next_value(&sim);
        int threshold    = register_store_get_holding(store, HOLDING_REG_THRESH_ADDR);

        register_store_set_input(store, INPUT_REG_SENSOR_ADDR, sensor_value);

        int alarm = alarm_evaluate(sensor_value, threshold);
        register_store_set_discrete(store, DISCRETE_ALARM_ADDR, alarm);

        int running = register_store_get_coil(store, COIL_RUNNING_ADDR);

        printf("Sensor: %-6d | Threshold: %-6d | Alarm: %d | Running: %d\n",
               sensor_value, threshold, alarm, running);

        sleep(SIM_INTERVAL_SEC);
    }

    return NULL;
}

int main(void)
{
    register_store_t store;

    if (register_store_init(&store) != 0) {
        fprintf(stderr, "Register store initialization failed\n");
        return EXIT_FAILURE;
    }

    pthread_t sim_tid;
    if (pthread_create(&sim_tid, NULL, simulation_loop, &store) != 0) {
        fprintf(stderr, "Failed to start simulation thread\n");
        register_store_destroy(&store);
        return EXIT_FAILURE;
    }
    pthread_detach(sim_tid);

    /* Blocks forever, serving Modbus requests */
    tcp_server_run(&store);

    /* Unreachable in normal operation, kept for completeness */
    register_store_destroy(&store);
    return EXIT_SUCCESS;
}
