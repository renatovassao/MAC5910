all: mqtt_server

OBJS= connect.o \
		connack.o \
		control_packet.o \
		disconnect.o \
		publish.o \
		suback.o \
		subscribe.o \
		handler.o \
		pingreq.o \
		pingresp.o \
		mqtt_server.o

connect.o: src/parser/connect.c
	gcc -Wall -c $< -o lib/$@

connack.o: src/parser/connack.c
	gcc -Wall -c $< -o lib/$@

control_packet.o: src/parser/control_packet.c
	gcc -Wall -c $< -o lib/$@

disconnect.o: src/parser/disconnect.c
	gcc -Wall -c $< -o lib/$@

publish.o: src/parser/publish.c
	gcc -Wall -c $< -o lib/$@

suback.o: src/parser/suback.c
	gcc -Wall -c $< -o lib/$@

subscribe.o: src/parser/subscribe.c
	gcc -Wall -c $< -o lib/$@

pingreq.o: src/parser/pingreq.c
	gcc -Wall -c $< -o lib/$@

pingresp.o: src/parser/pingresp.c
	gcc -Wall -c $< -o lib/$@

handler.o: src/handler.c
	gcc -Wall -c $< -o lib/$@

mqtt_server.o: src/mqtt_server.c
	gcc -Wall -c $< -o lib/$@

mqtt_server: ${OBJS}
	gcc -Wall lib/*.o -o bin/mqtt_server

clean:
	rm -f bin/* lib/*