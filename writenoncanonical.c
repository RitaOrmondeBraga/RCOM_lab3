/*Non-Canonical Input Processing*/

#include linklayer.h

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define FLAG 0x5c
#define A 0x03
#define C_SET 0x07
#define C_UA 0x06


volatile int STOP=FALSE;

int main(int argc, char** argv){
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255], byte;
    int i, sum = 0, speed = 0;
    unsigned char frame[5];

    if ( (argc < 2) ||
         ((strcmp("/dev/ttyS0", argv[1])!=0) &&
          (strcmp("/dev/ttyS1", argv[1])!=0) )) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        exit(1);
    }


    /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
    */


    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
        perror("tcgetattr");
        exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */


    /*
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a
    leitura do(s) próximo(s) caracter(es)
    */


    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }

    printf("New termios structure set\n");
    
    frame[0] = FLAG;
    frame[1] = A;
    frame[2] = C_SET;
    frame[3] = A^C_SET; //XOR
    frame[4] = FLAG; 

    //write SET
    res = write(fd, frame, 5); 
    printf("SET written\n");

 While (state != 5) {
        res = read(fd, &byte, 1);
        printf(":%x\n", byte[0]);

        switch (state) {
            case 0:
                if (byte != FLAG) {
                    state = 0;
                }
                else {
	                state = 1;
	              } 
                break;
            case 1:
                if (byte == FLAG) {
                    state = 1 
                }
                else if (byte == A) {
                    state = 2
                }
               else {
	               state = 0
               }
                break;
            case 2:
                if (byte == FLAG) {
                    state = 1;
                }
                else if (byte == C_UA) {
                    state = 3;
                }
               else {
	               state = 0;
                }
            }
                break;
            case 3:
                if (byte == (A^C_UA)) {
                    state = 4;
                }
                else if (byte == FLAG) {
                    state = 1;
                }
                else
                {
                    state = 0;
                }
                break;
            case 4:
                if (byte != FLAG) {
                    state = 0;
                }
                else
                {
                    state = 5;
                }
                break;
            default:
                break;
        }

    printf("UA received\n");


    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }


    close(fd);
    return 0;
}
