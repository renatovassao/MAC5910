#include "./parser/control_packet.h"

control_packet * control_packet_response(control_packet *cp);
int handle(control_packet *cp, char *pipe);