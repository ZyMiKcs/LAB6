#include "../include/clientfunc.h"

int main() {
    int client_sockfd;
    int len, result;
    struct sockaddr_in address;
    pthread_t thread_receive, thread_send;

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

    result = connect(client_sockfd, (struct sockaddr *)&address, (socklen_t)len);
    if (result == -1) {
        perror("Client error");
        exit(1);
    }

    printf("~~~~~SOCKET_CHAT~~~~~\n");

    pthread_create(&thread_receive, NULL, receive_msg, &client_sockfd);
    pthread_create(&thread_send, NULL, send_msg, &client_sockfd);

    pthread_join(thread_receive, NULL);
    pthread_join(thread_send, NULL);

    close(client_sockfd);
    exit(0);
}
