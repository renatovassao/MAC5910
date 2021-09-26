#include <stdio.h>
#include <stdlib.h>

#include "suback.h"

void print_suback_variable_header(suback_variable_header *svh) {
    printf("packet_identifier: 0x%x\n", svh->packet_identifier);
}

void print_suback_payload(suback_payload *sp) {
    printf("return_code: 0x%x\n", sp->return_code);
}

int suback_variable_header_length(control_packet *cp) {
    int type_flags_length = 1;
    int digits = remaining_length_digits(cp);
    int return_code_length = 1;

    return type_flags_length + digits + return_code_length;
}

int create_suback_fixed_header(control_packet *cp) {
    cp->fixed_header.type = SUBACK;
    cp->fixed_header.flags = 0x00;
    cp->fixed_header.remaining_length = 3;

    return 0;
}

int create_suback_variable_header(short packet_identifier, control_packet *cp) {
    suback_variable_header *svh = malloc(sizeof(suback_variable_header));

    svh->packet_identifier = packet_identifier;

    cp->variable_header = svh;

    return 0;
}

int create_suback_payload(RETURN_CODE rc, control_packet *cp) {
    suback_payload *sp = malloc(sizeof(suback_payload));

    sp->return_code = rc;

    cp->payload = sp;

    return 0;
}

control_packet * create_suback_control_packet(short packet_identifier, RETURN_CODE rc) {
    control_packet *cp = malloc(sizeof(control_packet));

    create_suback_fixed_header(cp);
    create_suback_variable_header(packet_identifier, cp);
    create_suback_payload(rc, cp);

    return cp;
}

int encode_suback_variable_header(control_packet *cp, ubyte *bytes) {
    int digits = remaining_length_digits(cp);

    suback_variable_header * svh = (suback_variable_header *) cp->variable_header;

    bytes[1 + digits] = msb(svh->packet_identifier);
    bytes[2 + digits] = lsb(svh->packet_identifier);

    return 0;
}

int encode_suback_payload(control_packet *cp, ubyte *bytes) {
    int digits = remaining_length_digits(cp);

    suback_payload * sp = (suback_payload *) cp->payload;

    bytes[3 + digits] = sp->return_code;

    return 0;
}

void free_suback_variable_header(suback_variable_header *svh) {
    free(svh);
}

void free_suback_payload(suback_payload *sp) {
    free(sp);
}