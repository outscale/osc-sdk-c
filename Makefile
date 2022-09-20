all: osc_sdk.c osc_sdk.h

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

