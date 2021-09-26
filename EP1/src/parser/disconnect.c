#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disconnect.h"

void print_disconnect_variable_header(disconnect_variable_header *svh) {
    printf("<empty>\n");
}

void print_disconnect_payload(disconnect_payload *sp) {
    printf("<empty>\n");
}

int decode_disconnect_variable_header(ubyte *data, control_packet *cp) {
    return 0;
}

int decode_disconnect_payload(ubyte *data, control_packet *cp) {
    return 0;
}