#ifndef CONNECT_H
#define CONNECT_H

#include "control_packet.h"

typedef enum {
    CLEAN_SESSION = 0x02,
} CONNECT_FLAG;

typedef struct {
    short protocol_length;
    char *protocol_name;
    PROTOCOL_LEVEL protocol_level;
    ubyte connect_flags;
    short keep_alive;
} connect_variable_header;

typedef struct {
    short client_id_length;
    char *client_id;
} connect_payload;

void print_connect_variable_header(connect_variable_header *cvh);
void print_connect_payload(connect_payload *cp);
int check_for_connect_flag(control_packet *cp, CONNECT_FLAG cf);
int decode_connect_variable_header(ubyte *data, control_packet *cp);
int decode_connect_payload(ubyte *data, control_packet *cp);
void free_connect_variable_header(connect_variable_header *cvh);
void free_connect_payload(connect_payload *cp);

#endif /* CONNECT_H */