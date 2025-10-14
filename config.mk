.DEFAULT_GOAL = build
.PHONY = clean build

CFLAGS += -std=c11 -O -s
CFLAGS += -Wall -Wextra -Werror -pedantic
CFLAGS += -D_FORTIFY_SOURCE=2 -D_DEFAULT_SOURCE

LDFLAGS += -O -s
