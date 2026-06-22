#ifndef REGISTER_STORE_H
#define REGISTER_STORE_H

#include <modbus/modbus.h>
#include <pthread.h>

/*
 * Owns the libmodbus register/coil mapping plus the mutex that protects
 * it. This is the ONLY place that knows the mapping exists — every other
 * module talks to registers through the accessor functions below.
 */
typedef struct {
    modbus_mapping_t *mapping;
    pthread_mutex_t   lock;
} register_store_t;

/* Allocates the mapping (sized from config.h) and sets default values. */
int  register_store_init(register_store_t *store);
void register_store_destroy(register_store_t *store);

/*
 * Raw lock/unlock + mapping accessor — used only by modbus_tcp.c, which
 * needs to hand the whole mapping to libmodbus's modbus_reply() atomically.
 * Everything else should use the typed accessors further down.
 */
void register_store_lock(register_store_t *store);
void register_store_unlock(register_store_t *store);
modbus_mapping_t *register_store_get_mapping(register_store_t *store);

/* Thread-safe single-value accessors for application code (sensor_sim,
 * alarm, main's simulation loop, etc.) — no mutex handling required. */
int  register_store_get_holding(register_store_t *store, int addr);
void register_store_set_holding(register_store_t *store, int addr, int value);

int  register_store_get_coil(register_store_t *store, int addr);
void register_store_set_coil(register_store_t *store, int addr, int value);

int  register_store_get_discrete(register_store_t *store, int addr);
void register_store_set_discrete(register_store_t *store, int addr, int value);

int  register_store_get_input(register_store_t *store, int addr);
void register_store_set_input(register_store_t *store, int addr, int value);

#endif /* REGISTER_STORE_H */
