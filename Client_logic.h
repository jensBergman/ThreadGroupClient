/* 
 * File:   Client_logic.h
 * Author: jesse
 *
 * Created on den 2 januari 2016, 12:53
 */

#ifndef CLIENT_LOGIC_H
#define	CLIENT_LOGIC_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <unistd.h> // declares read/write
#include <stdio.h>

#include "Safe_queue.h"

class Client_logic {
public:
    Client_logic();
    virtual ~Client_logic();
    
    static void readMessage(int sockfd, SafeQueue<std::string> &q_read);
    
    static void sendMessage(int sockfd, char*c);
private:

};

#endif	/* CLIENT_LOGIC_H */

