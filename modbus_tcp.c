#include "modbus_tcp.h"

int modbus_tcp_handle_request(modbus_t *ctx, register_store_t *store)
{
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

    int rc = modbus_receive(ctx, query);
    if (rc > 0) {
        register_store_lock(store);
        modbus_reply(ctx, query, rc, register_store_get_mapping(store));
        register_store_unlock(store);
    }

    return rc;
}
