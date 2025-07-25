all: help

help:
	@echo "Available targets:"
	@echo "- osc_sdk.a: build SDK as static library (you can also include the .c/.h direclty in your source)"
	@echo "- examples: build all examples"
	@echo "- integration-test: test if SDK works"
	@echo "- regen-test: test osc-sdk-c regeneration"
	@echo "- tests: run all tests"

examples: example0 example1 example2 example3 example_a

osc_sdk.o:
	gcc -c osc_sdk.c `pkg-config --cflags json-c` $(CFLAGS)

osc_sdk.a: osc_sdk.o
	ar -crs osc_sdk.a osc_sdk.o

example_a: osc_sdk.a osc_sdk.h examples/example_a.cc
	$(CXX) examples/example_a.cc -I./ -o example_a -Wall -Wno-unused-function -g $(CFLAGS) -lm `pkg-config --cflags jsoncpp` ./osc_sdk.a $(LDFLAGS) -lcurl `pkg-config --libs json-c jsoncpp`

example0: osc_sdk.a osc_sdk.h examples/example0.c
	$(CC) examples/example0.c -I./ -o example0 -Wall -Wno-unused-function -g -std=c89 $(CFLAGS) ./osc_sdk.a $(LDFLAGS) -lcurl -lm `pkg-config --libs json-c`

example1: osc_sdk.c osc_sdk.h examples/example1.c
	$(CC) examples/example1.c ./osc_sdk.c -I./ -lm `pkg-config --cflags json-c`  -o example1 -Wall -Wno-unused-function -g $(CFLAGS) $(LDFLAGS) `pkg-config --libs json-c` -lcurl

example2: osc_sdk.c osc_sdk.h examples/example2.c
	$(CC) examples/example2.c ./osc_sdk.c -I./ -lm `pkg-config --cflags json-c`  -o example2 -Wall -Wno-unused-function -g $(CFLAGS) $(LDFLAGS) `pkg-config --libs json-c` -lcurl

example3: osc_sdk.c osc_sdk.h examples/example3.c
	$(CC) examples/example3.c ./osc_sdk.c -I./ -lm `pkg-config --cflags json-c`  -o example3 -Wall -Wno-unused-function -g $(CFLAGS) $(LDFLAGS) `pkg-config --libs json-c` -lcurl

ricochet_preparation: osc_sdk.c osc_sdk.h examples/ricochet_preparation.c
	$(CC) examples/ricochet_preparation.c ./osc_sdk.c -I./ -lm `pkg-config --cflags json-c`  -o ricochet_preparation -Wall -Wno-unused-function -g $(CFLAGS) $(LDFLAGS) `pkg-config --libs json-c` -lcurl

COGNAC/.git:
	git submodule update --init

COGNAC/config.mk: COGNAC/.git
	@echo "==== make coniac with $$COGNAC_CONFIG ===="
	cd COGNAC && ./configure $$COGNAC_CONFIG

COGNAC/osc_sdk.c: COGNAC/config.mk
	make -C COGNAC osc_sdk.c

COGNAC/osc_sdk.h: COGNAC/config.mk
	make -C COGNAC osc_sdk.h

regen:  clean_sdk
	COGNAC_CONFIG="${COGNAC_CONFIG} $$(./tag-from-src.sh)" $(MAKE) COGNAC/config.mk
	printf "0X%02d%02d%02d\n" $$(cat sdk_version | cut -d '.' -f 1) $$(cat sdk_version | cut -d '.' -f 2) $$(cat sdk_version | cut -d '.' -f 3) > COGNAC/sdk-version
	make -j -C COGNAC osc_sdk.h osc_sdk.c
	cp COGNAC/osc_sdk.c .
	cp COGNAC/osc_sdk.h .

clean:
	rm -rvf examples/*.o example0 example1 example2 example3 example_a osc_sdk.o

clean_sdk:
	rm -rvf COGNAC/

fclean: clean
	rm -vf osc_sdk.a

clean_all: fclean clean_sdk

tests: integration-test
	@echo "all tests ok"

integration-test: examples
	./intergration-test.sh

local-tests: ricochet_preparation
	./local-tests.sh

regen-test: regen
	git add osc_sdk.c osc_sdk.h
	git diff --cached --exit-code
	git diff --exit-code

.PHONY: clean clean_sdk clean_all fclean all tests integration-test regen-test regen help examples
