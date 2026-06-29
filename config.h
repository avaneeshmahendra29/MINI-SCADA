#ifndef CONFIG_H
#define CONFIG_H

/* ---------------- Network configuration ---------------- */
#define SERVER_IP    "0.0.0.0"   /* listen on all interfaces; change to a
                                     fixed IP (e.g. "192.168.0.20") if you
                                     want to bind to one NIC only */
#define SERVER_PORT  502

/* ---------------- Datastore sizing ---------------- */
/* Matches the original Python prototype: 10 of each table */
#define NUM_COILS             10
#define NUM_DISCRETE_INPUTS   10
#define NUM_INPUT_REGISTERS   10
#define NUM_HOLDING_REGISTERS 10

/* ---------------- Register / coil map ---------------- */
/* Addresses are 0-based, same convention pymodbus uses */
#define COIL_RUNNING_ADDR        1   /* master writes 1 = "process running" */
#define DISCRETE_ALARM_ADDR      2   /* server writes 1 when sensor > threshold */
#define INPUT_REG_SENSOR_ADDR    1   /* server writes simulated sensor value here */
#define HOLDING_REG_THRESH_ADDR  1   /* master writes alarm threshold here */

/* ---------------- Simulation tuning ---------------- */
#define DEFAULT_THRESHOLD     500
#define SENSOR_CENTER         500
#define SENSOR_AMPLITUDE      500
#define SENSOR_ANGULAR_STEP   0.1
#define SIM_INTERVAL_SEC      1

#endif /* CONFIG_H */
