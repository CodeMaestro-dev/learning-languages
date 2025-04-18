#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// define the port where the sucket woulfd run on
#define PORT 9999

// define the buffer size for the memroy to store infomrmation temporarily
#define BUFFER_SIZE 1024

int main()
{
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>Hello, World!! This is my first C HTTP Server. I am so exciteed I was able to finish it.</html>\r\n";

    // create a socket at this point
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("webserver (socket)");
        return 1;
    }

    printf("socket created successfully\n");

    // create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // create the client adress
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    // now we bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0)
    {
        perror("webserver (bind)");
        return 1;
    }

    printf("socket successfully bouund to address\n");

    if (listen(sockfd, SOMAXCONN) != 0)
    {
        perror("webserver (listen)");
        return 1;
    }

    printf("server listening for connections\n");

    // this is an infinite for loop in C
    for (;;)
    {
        // accepting incoming connections
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
        if (newsockfd < 0)
        {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted \n");

        // get the client's address
        int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);

        if(sockn < 0)
        {
            perror("webserver (getsocketname)");
            continue;
        }

        // read from the socket
        int valread = read(newsockfd, buffer, BUFFER_SIZE);

        if (valread < 0)
        {
            perror("webserver (read)");
            continue;
        }

       
        // // read the request
        // char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        // sscanf(buffer, "%s %s %s")
        
        // write to the socket
        int valwrite = write(newsockfd, resp, strlen(resp));

        if (valwrite < 0)
        {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return 0;
}
