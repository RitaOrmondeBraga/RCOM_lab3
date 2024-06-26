/*Non-Canonical Input Processing*/

#include "linklayer.c"

volatile int STOP=FALSE;

int main(int argc, char** argv){

    if ( (argc < 2) ||
         ((strcmp("/dev/ttyS10", argv[1])!=0) &&
          (strcmp("/dev/ttyS11", argv[1])!=0) )) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        exit(1);
    }

    linkLayer connectionParameters;

    conncectionParameter.role = TRANSMITTER;   
    for (int i = 0; i < 12; i++)
        connectionParameters.serialPort[i] = argv[1][i];
    }


    llopen(connectionParameters);
    return 0;