#include "mongoose.h"

static void handle_request(struct mg_connection *conn) {
    mg_printf_data(conn, "Hello! Requested URI is [%s]", conn->uri);
}

static void handle_websocket(struct mg_connection *conn) {
    if (conn->content_len > 0) {
        mg_websocket_write(conn, 1, conn->content, conn->content_len);
    }
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
    switch (ev) {
    case MG_REQUEST:
        if (conn->is_websocket) {
            handle_websocket(conn);
        }
        else {
            handle_request(conn);
        }
        return MG_TRUE;
    case MG_WS_CONNECT:
        return MG_FALSE;
    case MG_CLOSE:
        return MG_TRUE;
    case MG_AUTH: 
        return MG_TRUE;
    default: 
        return MG_FALSE;
    }
}

int main(void) {

    struct mg_server *server;

    // Create and configure the server
    server = mg_create_server(NULL, ev_handler);
    mg_set_option(server, "listening_port", "8080");

    // Serve request. Hit Ctrl-C to terminate the program
    printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
    for (;;) {
        mg_poll_server(server, 1000);
    }

    // Cleanup, and free server instance
    mg_destroy_server(&server);

    return 0;
}