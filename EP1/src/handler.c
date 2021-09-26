#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "./parser/control_packet.h"
#include "./parser/connack.h"
#include "./parser/subscribe.h"
#include "./parser/suback.h"
#include "./parser/pingreq.h"
#include "./parser/pingresp.h"
#include "./parser/publish.h"

#define DATAPATH "/tmp/mqtt_server"
#define PIPELENGTH 24

control_packet * connect_response(control_packet *req) {
    return create_connack_control_packet(SESSION_PRESENT_0, CONNECTION_ACCEPTED);
}

control_packet * subscribe_response(control_packet *req) {
    subscribe_variable_header *svh = (subscribe_variable_header *) req->variable_header;

    return create_suback_control_packet(svh->packet_identifier, RETURN_CODE_0);
}

control_packet * pingreq_response(control_packet *req) {
    return create_pingresp_control_packet();
}

control_packet * control_packet_response(control_packet *cp) {
    switch (cp->fixed_header.type) {
        case CONNECT:
            return connect_response(cp);
        case SUBSCRIBE:
            return subscribe_response(cp);
        case PINGREQ:
            return pingreq_response(cp);
        case PUBLISH:
        case DISCONNECT:
            return NULL;

        default:
            return NULL;
    }
}

int subscribe_handle(control_packet *cp, char *pipe) {
    subscribe_payload *sp = (subscribe_payload *) cp->payload;

    char * filename = malloc(sizeof(char) * (PIPELENGTH + sp->topic_filter_length));
    sprintf(filename, "%s/topic_%s", DATAPATH, sp->topic_filter_name);
    FILE * fp = fopen((const char *) filename, "a");
    if (fp == NULL) {
        printf("bad fopen\n");
        return -1;
    }

    fprintf(fp, "%s\n", pipe);

    fclose(fp);
    free(filename);

    return 0;
}

int publish_handle(control_packet *cp) {
    size_t len = 0;
    ssize_t n;
    char * row = NULL;

    publish_variable_header * pvh = (publish_variable_header *) cp->variable_header;

    ubyte *bytes = malloc(sizeof(ubyte) * control_packet_length(cp));
    encode_control_packet(cp, bytes);

    char * filename = malloc(sizeof(char) * (PIPELENGTH + pvh->topic_length));
    sprintf(filename, "%s/topic_%s", DATAPATH, pvh->topic_name);

    FILE * fp = fopen((const char *) filename, "r");
    if (fp == NULL) {
        return 0;
    }

    while ((n = getline(&row, &len, fp)) != -1) {
        row[strlen(row) - 1] = 0;

        int fd = open((const char *) row, O_WRONLY);
        if (fd > 0) {
            write(fd, bytes, control_packet_length(cp));
            close(fd);
        }
    }

    fclose(fp);
    free(filename);
    free(bytes);
    free(row);

    return 0;
}

int disconnect_handle(char * pipe) {
    int pipe_fd = open(pipe, O_WRONLY);
    close(pipe_fd);
    unlink(pipe);
    return 0;
}

int handle(control_packet *cp, char *pipe) {
    switch (cp->fixed_header.type) {
        case SUBSCRIBE:
            return subscribe_handle(cp, pipe);
        case PUBLISH:
            return publish_handle(cp);
        case DISCONNECT:
            return disconnect_handle(pipe);
        case CONNECT:
        case CONNACK:
        case SUBACK:
        case PINGREQ:
        case PINGRESP:
            return 0;

        default:
            return -1;
    }
}