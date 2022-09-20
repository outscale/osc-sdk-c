example0: osc_sdk.c osc_sdk.h
	$(CC) examples/example0.c ./osc_sdk.c -I./ `pkg-config --cflags json-c` -lcurl -ljson-c -o example0

COGNAC/config.mk:
	cd COGNAC && ./configure

COGNAC/osc_sdk.c: COGNAC/config.mk
	make -C COGNAC osc_sdk.c

COGNAC/osc_sdk.h: COGNAC/config.mk
	make -C COGNAC osc_sdk.h

osc_sdk.c: COGNAC/osc_sdk.c
	cp COGNAC/osc_sdk.c .

osc_sdk.h: COGNAC/osc_sdk.h
	cp COGNAC/osc_sdk.h .

