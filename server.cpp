#include "iostream"
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAXLINE 1024

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error!! NO PORT PROVIDED\n");
        exit(1);
    }
    int portNo = atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        cout << "Failed to create socket. errno: " << endl;
        exit(1);
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(portNo); // htons convert a number to network byte order
    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        cout << "Failed to bind to port." << endl;
        exit(1);
    }

    struct sockaddr_in cliaddr;
    char buffer[MAXLINE];

    cout << "Listening..." << endl;
    if (listen(sockfd, 5) < 0)
    {
        cout << "Failed to listen on socket" << endl;
        exit(1);
    }
    auto addrlen = sizeof(sockaddr);
    int connection, pid;

    srand(time(NULL));

    while (1)
    {
        connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
        if (connection < 0)
        {
            cout << "Failed to grab connection." << endl;
            exit(1);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("Fork error!!");
            exit(1);
        }

        if (pid == 0)
        {
            // child process
            while (1)
            {
                char buffer[100];
                auto bytesRead = read(connection, buffer, 100);
                cout << "The message was: " << buffer << "\n";
                
                //generate tempeture reading
                char tempStr[4];
                int temp = rand() % 100 + 1;
                snprintf(tempStr, sizeof(tempStr), "%d", temp);

                send(connection, tempStr, sizeof(tempStr), 0);
            }
            close(connection);
            close(sockfd);
            exit(0);
        }
        else
        {
            // parent process
            close(connection);
        }
    }
    // Close the connections
    close(connection);
    close(sockfd);

    return 0;
}
