# See LICENSE file for copyright and license details.

include ../config.mk

LDFLAGS += -lX11

NAME = dwmstatus

SRC = ${NAME}.c $(wildcard status-*.c)
OBJ = ${SRC:.c=.o}

build: ${NAME}

${NAME}: ${OBJ}
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f ${NAME} ${OBJ}
