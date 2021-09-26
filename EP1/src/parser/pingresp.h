#ifndef PINGRESP_H
#define PINGRESP_H

#include "control_packet.h"

typedef void pingresp_variable_header;

typedef void pingresp_payload;

void print_pingresp_variable_header(pingresp_variable_header *cvh);
void print_pingresp_payload(pingresp_payload *cp);
control_packet * create_pingresp_control_packet();
int encode_pingresp_variable_header(control_packet *cp, ubyte *bytes);
int encode_pingresp_payload(control_packet *cp, ubyte *bytes);

#endif /* PINGRESP_H */