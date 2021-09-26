#ifndef DISCONNECT_H
#define DISCONNECT_H

#include "control_packet.h"

typedef void disconnect_variable_header;

typedef void disconnect_payload;

void print_disconnect_variable_header(disconnect_variable_header *cvh);
void print_disconnect_payload(disconnect_payload *cp);
int decode_disconnect_variable_header(ubyte *data, control_packet *cp);
int decode_disconnect_payload(ubyte *data, control_packet *cp);

#endif /* DISCONNECT_H */