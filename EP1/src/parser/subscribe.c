#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subscribe.h"

void print_subscribe_variable_header(subscribe_variable_header *svh) {
    printf("packet_identifier: %d\n", svh->packet_identifier);
}

void print_subscribe_payload(subscribe_payload *sp) {
    printf("topic_filter_length: %d\n", sp->topic_filter_length);
    printf("topic_filter_name: %s\n", sp->topic_filter_name);
    printf("requested_qos: 0x%x\n", sp->requested_qos);
}

int decode_subscribe_variable_header(ubyte *data, control_packet *cp) {
    int digits = remaining_length_digits(cp);
    subscribe_variable_header *svh = malloc(sizeof(subscribe_variable_header));

    svh->packet_identifier = string_length(data[1 + digits], data[2 + digits]);

    cp->variable_header = svh;

    return 0;
}

int decode_subscribe_payload(ubyte *data, control_packet *cp) {
    int digits = remaining_length_digits(cp);
    subscribe_payload *sp = malloc(sizeof(subscribe_payload));

    sp->topic_filter_length = string_length(data[3 + digits], data[4 + digits]);

    sp->topic_filter_name = malloc(sizeof(char) * sp->topic_filter_length);
    memcpy(sp->topic_filter_name, &data[5 + digits], sp->topic_filter_length);

    sp->requested_qos = data[5 + digits + sp->topic_filter_length];

    cp->payload = sp;

    return 0;
}

void free_subscribe_variable_header(subscribe_variable_header *svh) {
    free(svh);
}

void free_subscribe_payload(subscribe_payload *sp) {
    free(sp->topic_filter_name);
    free(sp);
}