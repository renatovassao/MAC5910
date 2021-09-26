#ifndef PINGREQ_H
#define PINGREQ_H

#include "control_packet.h"

typedef void pingreq_variable_header;

typedef void pingreq_payload;

void print_pingreq_variable_header(pingreq_variable_header *cvh);
void print_pingreq_payload(pingreq_payload *cp);
int decode_pingreq_variable_header(ubyte *data, control_packet *cp);
int decode_pingreq_payload(ubyte *data, control_packet *cp);

#endif /* PINGREQ_H */