#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H

#include "control_packet.h"

typedef struct {
    short packet_identifier;
} subscribe_variable_header;

typedef enum {
    REQUESTED_QoS_0   = 0x00,
    REQUESTED_QoS_1   = 0x02,
    REQUESTED_QoS_2   = 0x06,
} REQUESTED_QoS;

typedef struct {
    short topic_filter_length;
    char *topic_filter_name;
    REQUESTED_QoS requested_qos;
} subscribe_payload;

void print_subscribe_variable_header(subscribe_variable_header *cvh);
void print_subscribe_payload(subscribe_payload *cp);
int decode_subscribe_variable_header(ubyte *data, control_packet *cp);
int decode_subscribe_payload(ubyte *data, control_packet *cp);
void free_subscribe_variable_header(subscribe_variable_header *pvh);
void free_subscribe_payload(subscribe_payload *pp);

#endif /* SUBSCRIBE_H */