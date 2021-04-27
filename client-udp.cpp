#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#define SERVERPORT "4950"
#define HOST "localhost"

int main() {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6; 
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(HOST, SERVERPORT, &hints, &servinfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // Loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    // Send 10,000 message
    for (int i = 0; i < 1000000; i++) {
        std::string current_number = std::to_string(i);
        std::cout << "sending: " << current_number << std::endl;
        numbytes = sendto(sockfd, current_number.c_str(), sizeof current_number.c_str(), 0, p->ai_addr, p->ai_addrlen);
        if (numbytes == -1) {
            perror("talker: sendto");
            exit(1);
        }
    }

    freeaddrinfo(servinfo);
    close(sockfd);

    return 0;
}