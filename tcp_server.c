#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "tcp_server.h"
#include "modbus_tcp.h"
#include "config.h"

void tcp_server_run(register_store_t *store)
{
    modbus_t *ctx = modbus_new_tcp(SERVER_IP, SERVER_PORT);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create Modbus TCP context\n");
        exit(EXIT_FAILURE);
    }

    int server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        fprintf(stderr, "modbus_tcp_listen failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        exit(EXIT_FAILURE);
    }

    printf("Modbus TCP server started on %s:%d\n", SERVER_IP, SERVER_PORT);

    /*
     * Single-master accept loop, matching the project's current
     * architecture (one laptop polling one Pi). To support multiple
     * simultaneous masters, replace this with a select()-based loop over
     * an array of client sockets — only this function would need to change.
     */
    while (1) {
        printf("Waiting for a master to connect...\n");
        int client_socket = modbus_tcp_accept(ctx, &server_socket);
        if (client_socket == -1) {
            fprintf(stderr, "Accept failed: %s\n", modbus_strerror(errno));
            continue;
        }
        printf("Master connected.\n");

        while (1) {
            int rc = modbus_tcp_handle_request(ctx, store);
            if (rc == -1) {
                printf("Master disconnected.\n");
                break;
            }
        }

        modbus_close(ctx);
    }

    close(server_socket);
    modbus_free(ctx);
}
