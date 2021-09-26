#ifndef PUBLISH_H
#define PUBLISH_H

#include "control_packet.h"

typedef struct {
    short topic_length;
    char *topic_name;
} publish_variable_header;

typedef char * publish_payload;

void print_publish_variable_header(publish_variable_header *cvh);
void print_publish_payload(publish_payload pp);
int publish_variable_header_length(control_packet *cp);
int decode_publish_variable_header(ubyte *data, control_packet *cp);
int decode_publish_payload(ubyte *data, control_packet *cp);
control_packet * create_publish_control_packet(char * topic_name, short topic_length, publish_payload pp);
int encode_publish_variable_header(control_packet *cp, ubyte *bytes);
int encode_publish_payload(control_packet *cp, ubyte *bytes);
void free_publish_variable_header(publish_variable_header *pvh);
void free_publish_payload(publish_payload pp);

#endif /* PUBLISH_H */