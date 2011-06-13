
#include <stdio.h>
#include "mss-utils.h"

void mss_print_packet (MssPacket* packet)
{
    char c = packet->generic.packet_type;

    if (c == MSS_BUS) {
        printf("[BUS for %d]",packet->bus.slave_addr);
    }
    else if (c == MSS_NRQ) {
        printf("[NRQ]");
    }
    else if (c == MSS_DAT) {
        printf("[DAT from %d to %d, number: %d,size: %d]",
                packet->dat.src_addr,
                packet->dat.dst_addr,
                packet->dat.number,
                packet->dat.data_len
              );
    }
    else if (c == MSS_ACK) {
        printf("[ACK for number %d]",packet->ack.number);
    }
    else
        printf("[???]");

    fflush(stdout);
}

