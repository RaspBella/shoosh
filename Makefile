CFLAGS=-Wall -Wextra -Og -ggdb3
BUILDFLAGS=-Wall -Werror -O2
LINK=$(shell pkg-config --cflags --libs ncurses)

srcdir=src
srcs=$(wildcard ${srcdir}/*.c)
objs=$(patsubst ${srcdir}/%.c, %.o, ${srcs})
bin=shoosh

default: ${bin}

build:
	make CFLAGS="${BUILDFLAGS}"

${bin}: ${objs} 
	cc ${CFLAGS} ${LINK} $^ -o $@

%.o: ${srcdir}/%.c ${headers}
	cc ${CFLAGS} -c $< -o $@

clean:
	rm -f ${bin} ${objs}
