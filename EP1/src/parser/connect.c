#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connect.h"

void print_connect_variable_header(connect_variable_header *cvh) {
    printf("protocol_length: %d\n", cvh->protocol_length);
    printf("protocol_name: %s\n", cvh->protocol_name);
    printf("protocol_level: 0x%x\n", cvh->protocol_level);
    printf("connect_flags: 0x%x\n", cvh->connect_flags);
    printf("keep_alive: %d\n", cvh->keep_alive);
}

void print_connect_payload(connect_payload *cp) {
    printf("client_id_length: %d\n", cp->client_id_length);
    printf("client_id: %s\n", cp->client_id);
}

int check_for_connect_flag(control_packet *cp, CONNECT_FLAG cf) {
    return cp->fixed_header.flags & (cf > 0) ? 1 : 0;
}

int decode_connect_variable_header(ubyte *data, control_packet *cp) {
    int digits = remaining_length_digits(cp);
    connect_variable_header *cvh = malloc(sizeof(connect_variable_header));

    cvh->protocol_length = string_length(data[1 + digits], data[2 + digits]);

    cvh->protocol_name = malloc(sizeof(char) * cvh->protocol_length);
    memcpy(cvh->protocol_name, &data[3 + digits], cvh->protocol_length);

    cvh->protocol_level = data[3 + digits + cvh->protocol_length];
    cvh->connect_flags = data[4 + digits + cvh->protocol_length];
    cvh->keep_alive = string_length(data[5 + digits + cvh->protocol_length], data[6 + digits + cvh->protocol_length]);

    cp->variable_header = cvh;

    return 0;
}

int decode_connect_payload(ubyte *data, control_packet *cp) {
    int digits = remaining_length_digits(cp);
    connect_payload *conn_p = malloc(sizeof(connect_payload));

    conn_p->client_id_length = string_length(data[11 + digits], data[12 + digits]);

    conn_p->client_id = malloc(sizeof(char) * conn_p->client_id_length);
    memcpy(conn_p->client_id, &data[13 + digits], conn_p->client_id_length);

    cp->payload = conn_p;

    return 0;
}

void free_connect_variable_header(connect_variable_header *cvh) {
    free(cvh->protocol_name);
    free(cvh);
}

void free_connect_payload(connect_payload *cp) {
    free(cp->client_id);
    free(cp);
}