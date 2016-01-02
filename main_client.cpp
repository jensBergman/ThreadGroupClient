/* 
 * File:   main.cpp
 * Author: jesse
 *
 * Created on den 31 december 2015, 13:53
 */

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <unistd.h> // declares read/write
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // defines the structure hostent
#include <thread>

#include "Safe_queue.h"
#include "Client_logic.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {


    int sockfd, stream_length;
    int port = 9999;
    //struct sockaddr_in server_addr;
    // defines a host computer on the Internet
    struct hostent *server;
    
    // read-buffer
    char buffer[256];
    
    // internet addresses
    struct sockaddr_in server_addr, client_addr;
    
    /*
     * AF_INET = Internet domain
     * SOCK_STREAM = TCP socket
     * 0 = let the operating choose the most appropiate protocol (i.e. 3rd arg = protocol)
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        cout << "error opening socket!" << endl;
        return 0;
    }cout << "Socket opened!" << endl;
    
    server = gethostbyname("127.0.0.1");
    if (server == NULL)
    {
      cout << "ERROR, no such host" << endl;
      return 0;
    }cout << "Server host found!" << endl;
    
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_addr.sin_addr.s_addr,
          server->h_length);
    server_addr.sin_port = htons(port);
    
    if (connect(sockfd,(struct sockaddr *) &server_addr,sizeof(server_addr)) < 0){
        cout << "Error establishing connection!" << endl;
        return 0;
    }cout << "Connection established!" << endl;
    
    // this thread safe queue can be used to put read data from another thread and read
    // from this thread. (makes more sense when its used for responsive gui)
    SafeQueue<string> q_read;    
    /* this is a stupid implementation programmed just for simulating client.
     * Usually client has a gui, then send and read thread would be in event handlers
     * which would result in a responsive gui. 
    */ 
    for(char i = '0'; i<= '9';i++){
        // send number from 0 to 9 to server
        char c[] = {i};
        thread send (&Client_logic::sendMessage, sockfd, c); 
        //std::thread (&Client_logic::sendMessage, sockfd, c).detach();    
        
        thread read (&Client_logic::readMessage, sockfd, ref(q_read)); 
       // std::thread (&Client_logic::readMessage, sockfd, ref(q_read)).detach();    

        // wait until threads are done 
        send.join(); 
        read.join();
        string msg = q_read.dequeue();

        //sleep(2);
        cout << "Message from server: " << msg <<  i << endl;
    }
    
    stream_length = write(sockfd,"q",1);
    if(stream_length < 0){
        cout << "Error writing to socket!" << endl;
        return 0;
    } cout << "ended communication" << endl;
    
    close(sockfd);
    
    
    
    // wait to make sure that all output streams are finished before exiting
    cin.get();
    return 0;
}

