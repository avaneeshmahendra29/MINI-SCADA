#ifndef SCADA_MODBUS_TCP_H
#define SCADA_MODBUS_TCP_H

#include <modbus/modbus.h>
#include "register_store.h"

/*
 * Reads one Modbus PDU from ctx and replies against the register store.
 * This is the protocol layer only — it knows about Modbus function codes
 * and the register store, but nothing about sockets or accept loops.
 *
 * Returns:
 *   > 0  request handled normally
 *   0    request ignored (e.g. not addressed to us / broadcast)
 *   -1   read error or the master closed the connection
 */
int modbus_tcp_handle_request(modbus_t *ctx, register_store_t *store);

#endif /* SCADA_MODBUS_TCP_H */
