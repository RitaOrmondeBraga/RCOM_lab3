#include "link_layer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int llopen(linkLayer connectionParameters);

witch (state) {
            case 0:
                if (byte != 0x5c) {
                    state = 0;
                }
                else {
	                state = 1;
	              } 
                break;
            case 1:
                if (byte == 0x5c) {
                    state = 1 
                }
                if (byte == A) {
                    state = 2
                }
               else {
	               state = 0
               }
                break;
            case 2:
                if (byte == 0x5c) {
                    state = 1;
                }
                 if (byte == C) {
                    state = 4;
                }
               else {
	               state = 0;
                }
            }
                break;
            case 3:
                if (byte == (0x01 ^ 0x06)) {
                    
                }
                break;
            case 4:
                if (byte != 0x5c) {
                    
                }
                break;
            case 5:
            
            default:
                break;
        }

int llwrite(char* buf, int bufSize);

int llread(char* packet);

int llclose(linkLayer connectionParameters, int showStatistics);
