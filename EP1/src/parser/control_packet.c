#include <stdio.h>
#include <stdlib.h>

#include "connect.h"
#include "connack.h"
#include "publish.h"
#include "subscribe.h"
#include "suback.h"
#include "pingreq.h"
#include "pingresp.h"
#include "disconnect.h"


short string_length(ubyte msb, ubyte lsb) {
    return (msb << 8) + lsb;
}

short msb(int n) {
    return (n & 0xFF00) >> 8;
}

short lsb(int n) {
    return n & 0x00FF;
}

char *type_to_str(CONTROL_PACKET_TYPE type) {
    switch (type) {
        case 0x10: return "CONNECT";
        case 0x20: return "CONNACK";
        case 0x30: return "PUBLISH";
        // case 0x40: return "PUBACK";
        // case 0x50: return "PUBREC";
        // case 0x60: return "PUBREL";
        // case 0x70: return "PUBCOMP";
        case 0x80: return "SUBSCRIBE";
        case 0x90: return "SUBACK";
        // case 0xa0: return "UNSUBSCRIBE";
        // case 0xb0: return "UNSUBACK";
        case 0xc0: return "PINGREQ";
        case 0xd0: return "PINGRESP";
        case 0xe0: return "DISCONNECT";
        default:   return "UNKNOWN";
    };
}

void print_fixed_header(fixed_header *fh) {
    printf("type: %s\n", type_to_str(fh->type));
    printf("flags: %d\n", fh->flags);
    printf("remaining_length: %d\n", fh->remaining_length);
}

void print_variable_header(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            print_connect_variable_header((connect_variable_header *) cp->variable_header);
            break;

        case CONNACK:
            print_connack_variable_header((connack_variable_header *) cp->variable_header);
            break;

        case PUBLISH:
            print_publish_variable_header((publish_variable_header *) cp->variable_header);
            break;

        case SUBSCRIBE:
            print_subscribe_variable_header((subscribe_variable_header *) cp->variable_header);
            break;

        case SUBACK:
            print_suback_variable_header((suback_variable_header *) cp->variable_header);
            break;

        case DISCONNECT:
            print_disconnect_variable_header((disconnect_variable_header *) cp->variable_header);
            break;

        case PINGREQ:
            print_pingreq_variable_header((pingreq_variable_header *) cp->variable_header);
            break;

        case PINGRESP:
            print_pingresp_variable_header((pingresp_variable_header *) cp->variable_header);
            break;
    }
}

void print_payload(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            print_connect_payload((connect_payload *) cp->payload);
            break;

        case CONNACK:
            print_connack_payload((connack_payload *) cp->payload);
            break;

        case PUBLISH:
            print_publish_payload((publish_payload) cp->payload);
            break;

        case SUBSCRIBE:
            print_subscribe_payload((subscribe_payload *) cp->payload);
            break;

        case SUBACK:
            print_suback_payload((suback_payload *) cp->payload);
            break;

        case DISCONNECT:
            print_disconnect_payload((disconnect_payload *) cp->payload);
            break;

        case PINGREQ:
            print_pingreq_payload((pingreq_payload *) cp->payload);
            break;

        case PINGRESP:
            print_pingresp_payload((pingresp_payload *) cp->payload);
            break;
    }
}

void print_control_packet(control_packet *cp) {
    printf("----\nFIXED HEADER\n");
    print_fixed_header(&cp->fixed_header);

    printf("\nVARIABLE HEADER\n");
    print_variable_header(cp);

    printf("\nPAYLOAD\n");
    print_payload(cp);

    printf("-----\n\n");
}

int decode_type(ubyte *data, fixed_header *fh) {
    int type = data[0] & 0xF0;

    switch (type) {
        case CONNECT:
        case CONNACK:
        case PUBLISH:
        case SUBSCRIBE:
        case SUBACK:
        case PINGREQ:
        case PINGRESP:
        case DISCONNECT:
            fh->type = type;
            return 0;

        default:
            return -1;
    }
}

int decode_flags(ubyte *data, fixed_header *fh) {
    fh->flags = 0x00;

    return 0;
}

int decode_remaining_length(ubyte *data, fixed_header *fh) {
    int multiplier = 1;
    int value = 0;
    int i = 1;
    int encodedByte;

    do {
        encodedByte = data[i++];
        value += (encodedByte & 127) * multiplier;

        if (multiplier > 128 * 128 * 128) {
            return -1;
        }

        multiplier *= 128;
    } while ((encodedByte & 128) != 0);

    fh->remaining_length = value;

    return 0;
}

int decode_fixed_header(ubyte *data, fixed_header *fh) {
    if (decode_type(data, fh) == -1) {
        printf("bad type\n");
        return -1;
    }

    if (decode_flags(data, fh) == -1) {
        printf("bad flags\n");
        return -1;
    }

    if (decode_remaining_length(data, fh) == -1) {
        printf("bad remaining_length\n");
        return -1;
    }

    return 0;
}

int remaining_length_digits(control_packet *cp) {
    int digits = 0;
    int remaining_length = cp->fixed_header.remaining_length;

    do {
        digits += 1;
        remaining_length /= 128;
    } while (remaining_length > 0);

    return digits;
}

int variable_header_length(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case PUBLISH:
            return publish_variable_header_length(cp);

        default:
            return -1;
    }
}

int payload_length(control_packet *cp) {
    return cp->fixed_header.remaining_length - variable_header_length(cp);
}

int payload_offset(control_packet *cp) {
    int type_flags_length = 1;

    return type_flags_length + remaining_length_digits(cp) + variable_header_length(cp);
}

int decode_variable_header(ubyte *data, control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            return decode_connect_variable_header(data, cp);
        case PUBLISH:
            return decode_publish_variable_header(data, cp);
        case SUBSCRIBE:
            return decode_subscribe_variable_header(data, cp);
        case DISCONNECT:
            return decode_disconnect_variable_header(data, cp);
        case PINGREQ:
            return decode_pingreq_variable_header(data, cp);

        default:
            return -1;
    }

    return 0;
}

int decode_payload(ubyte *data, int n, control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            return decode_connect_payload(data, cp);
        case PUBLISH:
            return decode_publish_payload(data, cp);
        case SUBSCRIBE:
            return decode_subscribe_payload(data, cp);
        case DISCONNECT:
            return decode_disconnect_payload(data, cp);
        case PINGREQ:
            return decode_pingreq_payload(data, cp);

        default:
            return -1;
    }

    return 0;
}

int decode_control_packet(ubyte *data, int n, control_packet *cp) {
    if (decode_fixed_header(data, &cp->fixed_header) == -1) {
        printf("bad fixed header\n");
        return -1;
    }

    if (decode_variable_header(data, cp) == -1) {
        printf("bad variable header\n");
        return -1;
    }

    if (decode_payload(data, n, cp) == -1) {
        printf("bad payload\n");
        return -1;
    }

    return 0;
}

int encode_remaining_length(fixed_header *fh, ubyte *bytes) {
    int x = fh->remaining_length;
    int i = 1;

    do {
        bytes[i] = x % 128;
        x /= 128;

        if (x > 0) {
            bytes[i++] = x | 128;
        }
    } while (x > 0);

    return 0;
}

int encode_fixed_header(control_packet *cp, ubyte *bytes) {
    bytes[0] = cp->fixed_header.type | cp->fixed_header.flags;

    return encode_remaining_length(&cp->fixed_header, bytes);
}

int encode_variable_header(control_packet *cp, ubyte *bytes) {
    switch (cp->fixed_header.type) {
        case CONNACK:
            return encode_connack_variable_header(cp, bytes);
        case PUBLISH:
            return encode_publish_variable_header(cp, bytes);
        case SUBACK:
            return encode_suback_variable_header(cp, bytes);
        case PINGRESP:
            return encode_pingresp_variable_header(cp, bytes);

        default:
            return -1;
    }
}

int encode_payload(control_packet *cp, ubyte *bytes) {
    switch (cp->fixed_header.type) {
        case CONNACK:
            return encode_connack_payload(cp, bytes);
        case PUBLISH:
            return encode_publish_payload(cp, bytes);
        case SUBACK:
            return encode_suback_payload(cp, bytes);
        case PINGRESP:
            return encode_pingresp_payload(cp, bytes);

        default:
            return -1;
    }
}

int control_packet_length(control_packet *cp) {
    int type_flags_length = 1;
    int digits = remaining_length_digits(cp);

    return type_flags_length + digits + cp->fixed_header.remaining_length;
}

int encode_control_packet(control_packet *cp, ubyte *bytes) {
    if (encode_fixed_header(cp, bytes) < 0) {
        printf("bad fixed header\n");
        return -1;
    }

    if (encode_variable_header(cp, bytes) < 0) {
        printf("bad variable header\n");
        return -1;
    }

    if (encode_payload(cp, bytes) < 0) {
        printf("bad payload\n");
        return -1;
    }

    return 0;
}

void free_variable_header(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            return free_connect_variable_header((connect_variable_header *)cp->variable_header);
        case CONNACK:
            return free_connack_variable_header((connack_variable_header *)cp->variable_header);
        case PUBLISH:
            return free_publish_variable_header((publish_variable_header *)cp->variable_header);
        case SUBSCRIBE:
            return free_subscribe_variable_header((subscribe_variable_header *)cp->variable_header);
        case SUBACK:
            return free_suback_variable_header((suback_variable_header *)cp->variable_header);
        case DISCONNECT:
            return;

        default:
            break;
    }
}

void free_payload(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            return free_connect_payload((connect_payload *)cp->payload);
        case CONNACK:
            return free_connack_payload((connack_payload *)cp->payload);
        case PUBLISH:
            return free_publish_payload((publish_payload) cp->payload);
        case SUBSCRIBE:
            return free_subscribe_payload((subscribe_payload *)cp->payload);
        case SUBACK:
            return free_suback_payload((suback_payload *)cp->payload);
        case DISCONNECT:
            return;

        default:
            break;
    }
}

void free_control_packet(control_packet *cp) {
    free_variable_header(cp);
    free_payload(cp);
}