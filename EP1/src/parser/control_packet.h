#ifndef CONTROL_PACKET_H
#define CONTROL_PACKET_H

typedef unsigned char ubyte;

typedef enum {
    CONNECT     = 0x10,
    CONNACK     = 0x20,
    PUBLISH     = 0x30,
    // PUBACK      = 0x40,
    // PUBREC      = 0x50,
    // PUBREL      = 0x60,
    // PUBCOMP     = 0x70,
    SUBSCRIBE   = 0x80,
    SUBACK      = 0x90,
    // UNSUBSCRIBE = 0xa0,
    // UNSUBACK    = 0xb0,
    PINGREQ     = 0xc0,
    PINGRESP    = 0xd0,
    DISCONNECT  = 0xe0,
} CONTROL_PACKET_TYPE;

typedef enum {
    DUP     = 0x08,
    QoS_0   = 0x00,
    QoS_1   = 0x02,
    QoS_2   = 0x06,
    RETAIN  = 0x01,
} CONTROL_PACKET_FLAG;

typedef struct {
    CONTROL_PACKET_TYPE type;
    ubyte flags;
    int remaining_length;
} fixed_header;

typedef enum {
    v3_1_1 = 0x04,
} PROTOCOL_LEVEL;

typedef struct {
    fixed_header fixed_header;
    void *variable_header;
    void *payload;
} control_packet;

short string_length(ubyte msb, ubyte lsb);
short msb(int n);
short lsb(int n);
char *type_to_str(CONTROL_PACKET_TYPE type);
int decode_control_packet(ubyte *data, int n, control_packet *cp);
void print_control_packet(control_packet *cp);
int remaining_length_digits(control_packet *cp);
int payload_length(control_packet *cp);
int payload_offset(control_packet *cp);
int control_packet_length(control_packet *cp);
int encode_control_packet(control_packet *cp, ubyte *bytes);
void free_control_packet(control_packet *cp);

#endif /* CONTROL_PACKET_H */