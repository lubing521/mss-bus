
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "mss-bus.h"

char msg[100];
int rem_addr;
int what;
int sent;
int len;

void* sender_fun (void* thread_parms)
{
    for(;;) {
        sent = 0;
        scanf("%d %s", &rem_addr, msg);
        len = strlen(msg);
        what = 0;
    }
}

void* receiver_fun (void* thread_parms)
{
    uint8_t sender_addr;
    int msg_size;
    int bcast;
    char msg_rcv[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    what = 1;

    for (;;) {
        if (what) {
            msg_size = mss_slave_recv(&sender_addr, msg_rcv, &bcast, 1);
            if (0 < msg_size) {
                msg_rcv[msg_size] = '\0';
                printf("[Received: %d][FROM: %d][MSG: %s]\n", msg_size, sender_addr, msg_rcv);
            }
        }
        else {
            while (sent != len) {
                int res = mss_slave_send (rem_addr, msg+sent, len-sent);
                sent += res;
                if(sent != 0)
                    printf("[Sent][TO: %d] %d bytes\n", rem_addr, res);

                msg_size = mss_slave_recv(&sender_addr, msg_rcv, &bcast, 1);
                if (0 < msg_size) {
                    msg[msg_size] = '\0';
                    printf("[Received: %d][FROM: %d][MSG: %s]\n", msg_size, sender_addr, msg_rcv);
                    msg[0] = 0;
                }

            }
            what = 1;
        }
    }
}


int main (int argc, char** argv)
{
    uint8_t slave_addr;
    pthread_t sender, receiver;

    if (argc != 2) {
        fprintf(stderr, "ERROR: No slave address given!\n");
        return -1;
    }

    slave_addr = atoi(argv[1]);

    mss_init( "/dev/ttyS1", B9600 );
    mss_init_slave(slave_addr);
    printf("Slave INIT: #%d\n", slave_addr);

    pthread_create (&sender, NULL, &sender_fun, NULL);
    pthread_create (&receiver, NULL, &receiver_fun, NULL);

    pthread_join (sender, NULL);
    pthread_join (receiver, NULL);

    mss_close();

    return 0;
}

