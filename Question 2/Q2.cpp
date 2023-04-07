#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE idC
#define MAX_CLIENTS idB
#define MAX_SOCKETS (idA+idB)

// global variables
int server_socket, client_sockets[MAX_CLIENTS], num_clients = 0;
pthread_t threads[MAX_CLIENTS];

// thread function to handle communication with a client
void *client_thread(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int num_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (num_bytes <= 0) {
            // client disconnected
            close(client_socket);
            break;
        }

        // send message to all other clients
        for (int i = 0; i < num_clients; i++) {
            if (client_sockets[i] != client_socket) {
                send(client_sockets[i], buffer, num_bytes, 0);
            }
        }
    }

    // remove client from list of connected clients
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_socket) {
            for (int j = i; j < num_clients - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            num_clients--;
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    // create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }

    // bind server socket to local address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server started, waiting for connections...\n");

    // accept incoming connections and create a new thread for each client
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        // add new client to list of connected clients
        if (num_clients < MAX_CLIENTS) {
            client_sockets[num_clients] = client_socket;
            num_clients++;
        } else {
            printf("Maximum number of clients reached, connection refused\n");
            close(client_socket);
            continue;
        }

        // create a new thread to handle communication with the client
        if (pthread_create(&threads[num_clients - 1], NULL, client_thread, &client_socket) != 0) {
            perror("pthread_create");
            exit(1);
        }

        printf("Client connected, assigned ID %d\n", num_clients);
    }

    return 0;
}
