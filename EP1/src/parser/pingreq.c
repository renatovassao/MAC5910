#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pingreq.h"

void print_pingreq_variable_header(pingreq_variable_header *svh) {
    printf("<empty>\n");
}

void print_pingreq_payload(pingreq_payload *sp) {
    printf("<empty>\n");
}

int decode_pingreq_variable_header(ubyte *data, control_packet *cp) {
    return 0;
}

int decode_pingreq_payload(ubyte *data, control_packet *cp) {
    return 0;
}