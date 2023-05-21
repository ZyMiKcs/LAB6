#include "../include/serverfunc.h"

int main() {
    int server_sockfd, server_len;
    struct sockaddr_in server_address;
    fd_set readfds;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, (socklen_t)server_len);
    listen(server_sockfd, 5);

    start_server(readfds, server_sockfd);

    exit(0);
}
