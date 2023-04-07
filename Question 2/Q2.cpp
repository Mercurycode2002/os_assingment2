#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;
int main() {

    int client_socket;

    struct sockaddr_in server_address;

    char buffer[1024];

    

    client_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {

        cout<< "Error creating socket" << endl;

        return 1;

    }

    cout << "[+] client created succesfully :)"<<endl;

    memset(&server_address, '\0', sizeof(server_address));

    server_address.sin_family = AF_INET;

    server_address.sin_port = htons(8080);

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");



    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    cout<<"[+]Connected to the server "<<endl;

    

    recv(client_socket, buffer, 1024, 0);

    cout<<"[+]Data recieved "<<endl;



    return 0;

}

