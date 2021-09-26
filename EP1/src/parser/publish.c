#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "publish.h"

void print_publish_variable_header(publish_variable_header *pvh) {
    printf("topic_length: %d\n", pvh->topic_length);
    printf("topic_name: %s\n", pvh->topic_name);
}

void print_publish_payload(publish_payload pp) {
    printf("%s\n", pp);
}

int decode_publish_variable_header(ubyte *data, control_packet *cp) {
    int digits = remaining_length_digits(cp);
    publish_variable_header *pvh = malloc(sizeof(publish_variable_header));

    pvh->topic_length = string_length(data[1 + digits], data[2 + digits]);

    pvh->topic_name = malloc(sizeof(char) * pvh->topic_length + 1);
    memcpy(pvh->topic_name, &data[3 + digits], pvh->topic_length);
    pvh->topic_name[pvh->topic_length] = 0;

    cp->variable_header = pvh;

    return 0;
}

int decode_publish_payload(ubyte *data, control_packet *cp) {
    int n = payload_length(cp);
    publish_payload pp = malloc(sizeof(char) * n);

    memcpy(pp, &data[payload_offset(cp)], n);

    cp->payload = pp;

    return 0;
}

int publish_variable_header_length(control_packet *cp) {
    int msb_lsb = 2;
    publish_variable_header * pvh = (publish_variable_header *) cp->variable_header;

    return msb_lsb + pvh->topic_length;
}

int create_publish_fixed_header(control_packet *cp) {
    cp->fixed_header.type = PUBLISH;
    cp->fixed_header.flags = 0x00;
    cp->fixed_header.remaining_length = publish_variable_header_length(cp);

    return 0;
}

int create_publish_variable_header(char * topic_name, short topic_length, control_packet *cp) {
    publish_variable_header *pvh = malloc(sizeof(publish_variable_header));
    pvh->topic_length = topic_length;
    pvh->topic_name = topic_name;

    cp->variable_header = pvh;

    return 0;
}

int create_publish_payload(publish_payload pp, control_packet *cp) {
    publish_payload publ_p = malloc(sizeof(char) * payload_length(cp));

    memcpy(publ_p, pp, payload_length(cp));

    cp->payload = publ_p;

    return 0;
}

control_packet * create_publish_control_packet(char * topic_name, short topic_length, publish_payload pp) {
    control_packet *cp = malloc(sizeof(control_packet));

    create_publish_fixed_header(cp);
    create_publish_variable_header(topic_name, topic_length, cp);
    create_publish_payload(pp, cp);

    return cp;
}

int encode_publish_variable_header(control_packet *cp, ubyte *bytes) {
    int digits = remaining_length_digits(cp);

    publish_variable_header * pvh = (publish_variable_header *) cp->variable_header;

    bytes[1 + digits] = msb(pvh->topic_length);
    bytes[2 + digits] = lsb(pvh->topic_length);

    memcpy(&bytes[3 + digits], pvh->topic_name, pvh->topic_length);

    return 0;
}

int encode_publish_payload(control_packet *cp, ubyte *bytes) {
    memcpy(&bytes[payload_offset(cp)], cp->payload, payload_length(cp));

    return 0;
}

void free_publish_variable_header(publish_variable_header *pvh) {
    free(pvh->topic_name);
    free(pvh);
}

void free_publish_payload(publish_payload pp) {
    free(pp);
    printf("oiasdfae\n");
}