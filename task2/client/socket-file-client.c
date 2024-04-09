#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <filename>\n", argv[0]);
        exit(0);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];

    portno = atoi(argv[2]);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("소켓 생성 실패");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "존재하지 않는 호스트\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("connect() 실패");
    }

    // Send filename to server
    n = write(sockfd, argv[3], strlen(argv[3]));
    if (n < 0) {
        error("write() 실패");
    }

    // Open file to send
    FILE *file = fopen(argv[3], "rb");
    if (file == NULL) {
        error("File open 실패");
    }

    // Send file data to server
    while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (write(sockfd, buffer, n) < 0) {
            error("Socket에 write 실패");
        }
    }

    if (n < 0) {
        error("파일 읽어오기 실패");
    }

    fclose(file);
    close(sockfd);
    return 0;
}
