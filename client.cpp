
// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

#define MAXLINE 1024
#define TIMEOUT_MS 1000000
#define WRITE_INTERVAL 5


void writeLog(time_t startTime,int& cumulative_temp, char* current_temp_str){
    time_t now = time(0);
    char cumulative_temp_str[4], *average_temp_str;
    
    int period = now-startTime;
    int avg_temp=cumulative_temp/period;
    
    snprintf(cumulative_temp_str, sizeof(cumulative_temp_str), "%d", cumulative_temp);
    
    char *dt = ctime(&now);
    ofstream myfile("client_reading.txt",  std::ios_base::app);

    if (myfile.is_open()){
        myfile << dt <<"Reading recived... cumulative Tempeture: " << cumulative_temp_str<<" Average Tempeture: "<<avg_temp<<" \n";
        myfile.close();
    }
    else
        cout << "Unable to open file";
}

void updateCumulative(char* temp, int& Cumulative_temp){
    int curTemp = atoi(temp);
    Cumulative_temp+=curTemp;
}

int main(int argc, char *argv[])
{

    int n,cumulative_temp=0, cnt=0;
    time_t startTime=time(0);
    socklen_t len;

    if (argc < 2){
        fprintf(stderr, "Error!! NO PORT PROVIDED\n");
        exit(1);
    }
    int PORT = atoi(argv[1]);

    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {   
        printf("Socket creation error \n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    int connection = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (connection < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char *messgae = "what is the current tempeture please?";
    char serverReading[1024] = {0};

    while (1)
    {
        sleep(1);
        send(sock, messgae, strlen(messgae), 0);
        printf("message sent\n");
        read(sock, serverReading, 1024);
        printf("%s\n", serverReading);
        
        updateCumulative(serverReading, cumulative_temp);
        if(++cnt==WRITE_INTERVAL){
            writeLog(startTime,cumulative_temp,serverReading);
            cnt=0;
        }
    }
    close(sock);

    return 0;
}