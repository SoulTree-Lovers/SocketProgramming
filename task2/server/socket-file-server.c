#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("bind() 실패");
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept connection from client
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("accept() 실패");
    }

    // Receive filename from client
    bzero(buffer, BUFFER_SIZE);
    n = read(newsockfd, buffer, BUFFER_SIZE - 1);
    if (n < 0) {
        error("Socket read() 실패");
    }
    printf("File name received: %s\n", buffer);

    // Open or create file to save received data
    FILE *file = fopen(buffer, "wb");
    if (file == NULL) {
        error("File open 실패");
    }

    // Receive data from client and save to file
    while ((n = read(newsockfd, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, sizeof(char), n, file);
        bzero(buffer, BUFFER_SIZE);
    }

    if (n < 0) {
        error("Socket read() 실패");
    }

    printf("File transfer complete.\n");

    fclose(file);
    close(newsockfd);
    close(sockfd);
    return 0;
}
