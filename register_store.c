#include <stdio.h>
#include <errno.h>
#include "register_store.h"
#include "config.h"

int register_store_init(register_store_t *store)
{
    store->mapping = modbus_mapping_new(NUM_COILS,
                                         NUM_DISCRETE_INPUTS,
                                         NUM_HOLDING_REGISTERS,
                                         NUM_INPUT_REGISTERS);
    if (store->mapping == NULL) {
        fprintf(stderr, "Failed to allocate modbus mapping: %s\n",
                modbus_strerror(errno));
        return -1;
    }

    if (pthread_mutex_init(&store->lock, NULL) != 0) {
        fprintf(stderr, "Failed to init register store mutex\n");
        modbus_mapping_free(store->mapping);
        store->mapping = NULL;
        return -1;
    }

    /* Defaults — mirrors the original Python prototype */
    store->mapping->tab_registers[HOLDING_REG_THRESH_ADDR] = DEFAULT_THRESHOLD;

    return 0;
}

void register_store_destroy(register_store_t *store)
{
    if (store->mapping) {
        modbus_mapping_free(store->mapping);
        store->mapping = NULL;
    }
    pthread_mutex_destroy(&store->lock);
}

void register_store_lock(register_store_t *store)
{
    pthread_mutex_lock(&store->lock);
}

void register_store_unlock(register_store_t *store)
{
    pthread_mutex_unlock(&store->lock);
}

modbus_mapping_t *register_store_get_mapping(register_store_t *store)
{
    return store->mapping;
}

int register_store_get_holding(register_store_t *store, int addr)
{
    pthread_mutex_lock(&store->lock);
    int value = store->mapping->tab_registers[addr];
    pthread_mutex_unlock(&store->lock);
    return value;
}

void register_store_set_holding(register_store_t *store, int addr, int value)
{
    pthread_mutex_lock(&store->lock);
    store->mapping->tab_registers[addr] = value;
    pthread_mutex_unlock(&store->lock);
}

int register_store_get_coil(register_store_t *store, int addr)
{
    pthread_mutex_lock(&store->lock);
    int value = store->mapping->tab_bits[addr];
    pthread_mutex_unlock(&store->lock);
    return value;
}

void register_store_set_coil(register_store_t *store, int addr, int value)
{
    pthread_mutex_lock(&store->lock);
    store->mapping->tab_bits[addr] = value;
    pthread_mutex_unlock(&store->lock);
}

int register_store_get_discrete(register_store_t *store, int addr)
{
    pthread_mutex_lock(&store->lock);
    int value = store->mapping->tab_input_bits[addr];
    pthread_mutex_unlock(&store->lock);
    return value;
}

void register_store_set_discrete(register_store_t *store, int addr, int value)
{
    pthread_mutex_lock(&store->lock);
    store->mapping->tab_input_bits[addr] = value;
    pthread_mutex_unlock(&store->lock);
}

int register_store_get_input(register_store_t *store, int addr)
{
    pthread_mutex_lock(&store->lock);
    int value = store->mapping->tab_input_registers[addr];
    pthread_mutex_unlock(&store->lock);
    return value;
}

void register_store_set_input(register_store_t *store, int addr, int value)
{
    pthread_mutex_lock(&store->lock);
    store->mapping->tab_input_registers[addr] = value;
    pthread_mutex_unlock(&store->lock);
}
