all: example0 example1 example2 example3

osc_sdk.o:
	gcc -c osc_sdk.c `pkg-config --cflags json-c`

osc_sdk.a: osc_sdk.o
	ar -crs osc_sdk.a  osc_sdk.o

example0: osc_sdk.a osc_sdk.h examples/example0.c
	$(CC) examples/example0.c ./osc_sdk.a -I./ -lcurl -ljson-c -o example0 -Wall -Wno-unused-function -g -std=c89 $(CFLAGS)

example1: osc_sdk.c osc_sdk.h examples/example1.c
	$(CC) examples/example1.c ./osc_sdk.c -I./ `pkg-config --cflags json-c` -lcurl -ljson-c -o example1 -Wall -Wno-unused-function -g $(CFLAGS)

example2: osc_sdk.c osc_sdk.h examples/example2.c
	$(CC) examples/example2.c ./osc_sdk.c -I./ `pkg-config --cflags json-c` -lcurl -ljson-c -o example2 -Wall -Wno-unused-function -g $(CFLAGS)

example3: osc_sdk.c osc_sdk.h examples/example3.c
	$(CC) examples/example3.c ./osc_sdk.c -I./ `pkg-config --cflags json-c` -lcurl -ljson-c -o example3 -Wall -Wno-unused-function -g $(CFLAGS)

COGNAC/.git:
	git submodule update --init

COGNAC/config.mk: COGNAC/.git
	cd COGNAC && ./configure

COGNAC/osc_sdk.c: COGNAC/config.mk
	make -C COGNAC osc_sdk.c

COGNAC/osc_sdk.h: COGNAC/config.mk
	make -C COGNAC osc_sdk.h

osc_sdk.c: COGNAC/osc_sdk.c
	cp COGNAC/osc_sdk.c .

osc_sdk.h: COGNAC/osc_sdk.h
	cp COGNAC/osc_sdk.h .

clean:
	rm -rvf examples/*.o example0 example1 example2 example3

clean_sdk:
	rm -rvf COGNAC/

clean_all: clean clean_sdk

test: example0 example1 example2 example3
	./intergration-test.sh

.PHONY: clean clean_sdk clean_all all test
