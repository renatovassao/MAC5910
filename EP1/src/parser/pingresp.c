#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pingresp.h"

void print_pingresp_variable_header(pingresp_variable_header *svh) {
    printf("<empty>\n");
}

void print_pingresp_payload(pingresp_payload *sp) {
    printf("<empty>\n");
}

int create_pingresp_fixed_header(control_packet *cp) {
    cp->fixed_header.type = PINGRESP;
    cp->fixed_header.flags = 0x00;
    cp->fixed_header.remaining_length = 0;

    return 0;
}

control_packet * create_pingresp_control_packet() {
    control_packet *cp = malloc(sizeof(control_packet));

    create_pingresp_fixed_header(cp);

    return cp;
}

int encode_pingresp_variable_header(control_packet *cp, ubyte *bytes) {
    return 0;
}

int encode_pingresp_payload(control_packet *cp, ubyte *bytes) {
    return 0;
}