/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
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

    frame[0] = 0x5c;
    frame[1] = 0x03;
    frame[2] = 0x08;
    frame[3] = 0x03^0x08; // XOR between A and C 
    frame[4] = 0x5c;

    /* for (i = 0; i < 255; i++) {
        buf[i] = 'a';
    } 

    //testing
    buf[25] = '\n'; 
    */

    res = write(fd, frame, 5); 
    // res = write(fd,buf,255);
    printf("%d bytes written\n", res);

    for (int i=0 ; i<5 ; i++) {       /* loop for input */
        res = read(fd, &byte, 1);  
        printf(":%s\n", byte);
        if (byte!=0x5c && i==0)
        {
            print("ERROR");
            exit (-1);
        }
        if (byte!=0x01 && i==1)
        {
            print("ERROR");
            exit (-1);
        }
        if (byte!=0x06 && i==2)
        {
            print("ERROR");
            exit (-1);
        }
        if (byte!=0x01^0x06 && i==3)
        {
            print("ERROR");
            exit (-1);
        }
        if (byte!=0x5c && i==4)
        {
            print("ERROR");
            exit (-1);
        }
    }

    /*
    O ciclo FOR e as instruções seguintes devem ser alterados de modo a respeitar
    o indicado no guião
    */


    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }


    close(fd);
    return 0;
}
