#include "serverfunc.h"

void start_server(fd_set readfds, int server_sockfd) {
    int fd_count = INIT_CLIENT;
    int client_len, client_sockfd;
    struct sockaddr_in client_address;

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while (1) {
        fd_set testfds = readfds;

        int result = select(fd_count, &testfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);
        if (result == -1) {
            perror("Server");
            exit(1);
        }

        for (int fd = 0; fd < fd_count; fd++) {
            if (FD_ISSET(fd, &testfds)) {
                if (fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd =
                        accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
                    FD_SET(client_sockfd, &readfds);

                    int user_num = client_sockfd - 3;
                    printf("[User-%d connected]\n", user_num);
                    fd_count++;
                } else {
                    int size;
                    ioctl(fd, FIONREAD, &size);

                    char *msg = malloc(sizeof(char) * size);
                    read(fd, msg, size);

                    if (strcmp(msg, "/exit") == 0) {
                        printf("[User-%d disconnected]\n", fd - 3);

                        close(fd);
                        FD_CLR(fd, &readfds);
                        fd_count--;
                    } else if (strcmp(msg, "/members") == 0) {
                        int code = 1;
                        int count_of_users = fd_count - INIT_CLIENT;

                        printf("[Sending list of chat members to User-%d]\n", fd - 3);

                        write(fd, &code, sizeof(int));
                        write(fd, &(count_of_users), sizeof(int));
                        for (int i = INIT_CLIENT; i < fd_count; i++) {
                            write(fd, &i, sizeof(int));
                        }
                    } else {
                        printf(" User-%d: %s\n", fd - 3, msg);

                        for (int fd_i = INIT_CLIENT; fd_i < fd_count; fd_i++) {
                            if (fd_i != fd) {
                                int code = 0;
                                int size1 = size + 15;

                                write(fd_i, &code, sizeof(int));
                                write(fd_i, &size1, sizeof(int));

                                char *buf = malloc(sizeof(char) * size1);
                                sprintf(buf, "User-%d: %s", fd - 3, msg);
                                write(fd_i, buf, size1);

                                free(buf);
                            }
                        }
                    }
                    free(msg);
                }
            }
        }
    }
}