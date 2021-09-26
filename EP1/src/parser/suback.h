#ifndef SUBACK_H
#define SUBACK_H

#include "control_packet.h"

typedef struct {
    short packet_identifier;
} suback_variable_header;

typedef enum {
    RETURN_CODE_0   = 0x00,
    RETURN_CODE_1   = 0x01,
    RETURN_CODE_2   = 0x02,
    FAILURE         = 0x80,
} RETURN_CODE;

typedef struct {
    RETURN_CODE return_code;
} suback_payload;

void print_suback_variable_header(suback_variable_header *cvh);
void print_suback_payload(suback_payload *cp);
control_packet * create_suback_control_packet(short packet_identifier, RETURN_CODE rc);
int encode_suback_variable_header(control_packet *cp, ubyte *bytes);
int encode_suback_payload(control_packet *cp, ubyte *bytes);
void free_suback_variable_header(suback_variable_header *pvh);
void free_suback_payload(suback_payload *pp);

#endif /* SUBACK_H */