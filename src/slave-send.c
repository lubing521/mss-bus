
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mss-bus.h"

int main( int argc, char** argv ) {
    int loc_addr;
    int rem_addr;
    int sent;
    int len;

    if( argc!=4 ) {
        printf("Liczba podanych parametrow jest ZLA!!!\n");
        return 0;
    }

    loc_addr = atoi(argv[1]);
    rem_addr = atoi(argv[2]);

    mss_init( "/dev/ttyS1", B9600 );
    mss_init_slave( loc_addr );

    printf("Slave-sender estabilished, transmission: %d=>%d\n", loc_addr, rem_addr);

    sent = 0;
    len = strlen(argv[3]);

    while( sent != len ) {
        int res = mss_slave_send ( rem_addr, argv[3]+sent, len-sent );
        sent += res;
        if(sent != 0)
            printf("Bytes transferred: %d\n", res);
    }

    mss_close();

    return 0;
}

