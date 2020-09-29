CC=clang

WARN= -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wformat-security -Wformat -Werror=format-security -Wstack-protector
SEC= -march=native -fstack-protector-all --param ssp-buffer-size=4 -fpie -ftrapv -D_FORTIFY_SOURCE=2

CFLAGS= ${SEC} ${WARN} -std=c99 -pedantic -O2 -I/usr/local/include -I/usr/local/include/freetype2 -DXINERAMA
LDFLAGS= -L/usr/local/lib -lX11 -lXft -lXinerama -lfontconfig -Wl,-z,relro,-z,now -pie

SRC = ./src/src.c ./src/swm.c ./src/util.c
OBJECT = src.o swm.o util.o

all: _swm_
	@echo CC -o swm
	@${CC} ${OBJECT} -o swm ${LDFLAGS}

_swm_:
	@echo CC ${SRC}
	@${CC} -c ${CFLAGS} ${SRC}

.PHONY: clean install uninstall

clean:
	@echo cleaning
	@rm -rf swm *.o *~

install: all
	@echo installing executable file to /usr/local/bin
	@mkdir -p /usr/local/bin
	@cp -f swm /usr/local/bin
	@chmod 755 /usr/local/bin/swm

uninstall:
	@echo removing executable file from /usr/bin
	@rm -f /usr/bin/swm
