#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <future>

using namespace std;

bool cf = false;
string cbuf;
void CIN()
{
    cin >> cbuf;
    cf = true;
}

int main()
{
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char buf[bufsize];

    struct sockaddr_in server_addr;
    socklen_t size;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    listen(client, 1);

    int clientCount = 1;
    server = accept(client, (struct sockaddr *)&server_addr, &size);

    if (server < 0) 
        cout << "=> Error on accepting..." << endl;
    
    cout << "ready" << endl;

    auto f = async(launch::async, CIN);

    while (server > 0)
    {
        // send(server, "connected\n", 10, 0);
        if (cf == true)
        {
            cout << "SERVER => " << cbuf << endl;
            int len = cbuf.size();
            strcpy(buf, cbuf.c_str()); 
            send(server, buf, len, 0);
            cf = false;
        }
        recv(server, buffer, bufsize, 0);
        cout << buffer << endl;
        strcpy(buffer, "");
        
    }
    close(client);
    close(server);
    return 0;
}