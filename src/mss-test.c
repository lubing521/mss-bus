
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "mss-bus.h"

void* sender(void* thread_parms) {
	int rem_addr;
	char msg[100];
	int sent;
	int len;
	while(1) {
		sent = 0;
		scanf("%d %s", &rem_addr, msg);
		len = strlen(msg);
		
		while( sent != len ) {
			int res = mss_slave_send ( rem_addr, msg+sent, len-sent );
			sent += res;
			if(sent != 0)
				printf("|| [Sent][TO: %d] %d bytes\n", rem_addr, res);
		}
	}
}

void* receiver(void* thread_parms) {
	uint8_t sender_addr;
    int msg_size;
    int bcast;
	char msg[100];
	while(1) {
		msg_size = mss_slave_recv(&sender_addr, msg, &bcast);
		msg[msg_size] = '\0';
		printf("|| [Received][FROM: %d] %s ||", sender_addr, msg);
		msg[0] = 0;
	}
}


int main (int argc, char** argv)
{
    uint8_t slave_addr;
    
    
    if (argc != 2) {
		fprintf(stderr, "ERROR: No slave address given!\n");
		return -1;
    }

    slave_addr = atoi(argv[1]);

    mss_init( "/dev/ttyS1", B9600 );
    mss_init_slave(slave_addr);
    printf("Slave INIT: #%d\n", slave_addr);

	pthread_t sender, receiver;
    pthread_create (&sender, NULL, &sender, NULL);
	pthread_create (&receiver, NULL, &receiver, NULL);
	
	pthread_join (sender, NULL);
	pthread_join (receiver, NULL);

    mss_close();
    
    return 0;
}

