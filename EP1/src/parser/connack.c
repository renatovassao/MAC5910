#include <stdio.h>
#include <stdlib.h>

#include "connack.h"

void print_connack_variable_header(connack_variable_header *cvh) {
    printf("session_present: 0x%x\n", cvh->session_present);
    printf("connect_return_code: 0x%x\n", cvh->connect_return_code);
}

void print_connack_payload(connack_payload *pp) {
    printf("<empty>\n");
}

int create_connack_fixed_header(control_packet *cp) {
    cp->fixed_header.type = CONNACK;
    cp->fixed_header.flags = 0x00;
    cp->fixed_header.remaining_length = 2;

    return 0;
}

int create_connack_variable_header(SESSION_PRESENT sp, CONNECT_RETURN_CODE crc, control_packet *cp) {
    connack_variable_header *cvh = malloc(sizeof(connack_variable_header));
    cvh->session_present = sp;
    cvh->connect_return_code = crc;

    cp->variable_header = cvh;

    return 0;
}

int create_connack_payload(control_packet *cp) {
    return 0;
}

control_packet * create_connack_control_packet(SESSION_PRESENT sp, CONNECT_RETURN_CODE crc) {
    control_packet *cp = malloc(sizeof(control_packet));

    create_connack_fixed_header(cp);
    create_connack_variable_header(sp, crc, cp);
    create_connack_payload(cp);

    return cp;
}

int encode_connack_variable_header(control_packet *cp, ubyte *bytes) {
    connack_variable_header * cvh = (connack_variable_header *) cp->variable_header;

    bytes[2] = cvh->session_present;
    bytes[3] = cvh->connect_return_code;

    return 0;
}

int encode_connack_payload(control_packet *cp, ubyte *bytes) {
    return 0;
}

void free_connack_variable_header(connack_variable_header *cvh) {
    free(cvh);
}

void free_connack_payload(connack_payload *cp) {
    return;
}