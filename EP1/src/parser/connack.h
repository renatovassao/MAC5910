#ifndef CONNACK_H
#define CONNACK_H

#include "control_packet.h"

typedef enum {
    SESSION_PRESENT_0 = 0x00,
    SESSION_PRESENT_1 = 0x01,
} SESSION_PRESENT;

typedef enum {
    CONNECTION_ACCEPTED                                 = 0x00,
    CONNECTION_REFUSED_UNACCEPTABLE_PROTOCOL_VERSION    = 0x01,
    CONNECTION_REFUSED_UNACCEPTABLE_IDENTIFIER_REJECTED = 0x02,
    CONNECTION_REFUSED_SERVER_UNAVAILABLE               = 0x03,
    CONNECTION_REFUSED_BAD_USERNAME_OR_PASSWORD         = 0x04,
    CONNECTION_REFUSED_NOT_AUTHORIZED                   = 0x05,
} CONNECT_RETURN_CODE;

typedef struct {
    SESSION_PRESENT session_present;
    CONNECT_RETURN_CODE connect_return_code;
} connack_variable_header;

typedef void connack_payload;

void print_connack_variable_header(connack_variable_header *cvh);
void print_connack_payload(connack_payload *cp);
control_packet * create_connack_control_packet(SESSION_PRESENT sp, CONNECT_RETURN_CODE crc);
int encode_connack_variable_header(control_packet *cp, ubyte *bytes);
int encode_connack_payload(control_packet *cp, ubyte *bytes);
void free_connack_variable_header(connack_variable_header *cvh);
void free_connack_payload(connack_payload *cp);

#endif /* CONNACK_H */