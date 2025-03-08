CFLAGS=-Wall -Wextra -Og -ggdb3
BUILDFLAGS=-Wall -Werror -O2
LINK=$(shell pkg-config --cflags --libs ncurses)

srcs=$(wildcard *.c)
objs=$(patsubst %.c, %.o, ${srcs})
headers=$(wildcard *.h)
bin=shoosh

default: ${bin}

build:
	make CFLAGS="${BUILDFLAGS}"

${bin}: ${objs} 
	cc ${CFLAGS} ${LINK} $^ -o $@

%.o: %.c ${headers}
	cc ${CFLAGS} -c $< -o $@

clean:
	rm -f ${bin} ${objs}
