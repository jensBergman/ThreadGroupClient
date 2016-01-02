/* 
 * File:   Client_logic.cpp
 * Author: jesse
 * 
 * Created on den 2 januari 2016, 12:53
 */

#include "Client_logic.h"

using namespace std;

Client_logic::Client_logic() {
}

Client_logic::~Client_logic() {
}

void Client_logic::readMessage(int sockfd, SafeQueue<string> &q_read){
    //while(true){
    // read-buffer
    char buffer[256];
    int stream_length;
    
    bzero(buffer, 256);
   // stream_length = read(sockfd, buffer, 255);
    stream_length = recv(sockfd, buffer, 19, MSG_WAITALL);
    if(stream_length < 0){
        cout << "Error reading from socket!" << endl;
        return;
    }
    //cout << "Message from server: " << buffer << endl;
    
    string temp = buffer;
    q_read.enqueue(temp);
    //}
}

void Client_logic::sendMessage(int sockfd, char*c){
    int stream_length;
    stream_length = write(sockfd,c,1);
        if(stream_length < 0){
            cout << "Error writing to socket!" << endl;
            return;
        } cout << "Message sent to server!" << endl;
}
