#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

const string CST_DBSERVER_ADDRESS = "127.0.0.1";
const unsigned short CST_DBSERVER_PORT = 6000;
const int CST_SOCKBUFF_SIZE = 1024;
const string CST_DBOP_EXIT_L = "e";
const string CST_DBOP_EXIT_U = "E";

int mainc(int argc, const char * argv[]) {
    
    int sock_client;
    char buff_recv[CST_SOCKBUFF_SIZE], buff_send[CST_SOCKBUFF_SIZE];
    in_addr_t nIPAddr;
    inet_pton(AF_INET, CST_DBSERVER_ADDRESS.c_str(), &nIPAddr);
    struct sockaddr_in addr_server;
    addr_server.sin_addr.s_addr = nIPAddr;
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(CST_DBSERVER_PORT);
    
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock_client, (struct sockaddr *)&addr_server, sizeof(struct sockaddr)) == 0) {
        char pIPAddr[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr_server.sin_addr.s_addr, pIPAddr, INET6_ADDRSTRLEN);
        cout << "socket connect to server "
        << pIPAddr << ":"
        << ntohs(addr_server.sin_port) << " success." << endl;
    }
    else {
        cout << "connect server failed." << endl;
        close(sock_client);
        exit(EXIT_FAILURE);
    }
    
    for ( ; ; ) {
        if (recv(sock_client, buff_recv, sizeof(buff_recv), 0) > 0) {
            cout << buff_recv << endl;
        }
        
        cin >> buff_send;
        if (CST_DBOP_EXIT_L.compare(buff_send) == 0
            || CST_DBOP_EXIT_U.compare(buff_send) == 0) {

            send(sock_client, buff_send, strlen(buff_send) + 1, 0);
            break;
        }
        
        send(sock_client, buff_send, strlen(buff_send) + 1, 0);
    }
    close(sock_client);
    
    cout << "Client is exit.\n";
    return 0;
}
