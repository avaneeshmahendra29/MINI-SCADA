#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "register_store.h"

/*
 * Binds to SERVER_IP:SERVER_PORT and loops forever: accept one master,
 * hand each incoming request to modbus_tcp_handle_request() until it
 * disconnects, then accept again. Blocks the calling thread forever.
 */
void tcp_server_run(register_store_t *store);

#endif /* TCP_SERVER_H */
