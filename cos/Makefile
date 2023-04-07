SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: vulnerable_server

vulnerable_server: vulnerable_server.o
	gcc -fno-stack-protector -z execstack -fno-builtin -mpreferred-stack-boundary=4 -o $@ $^

%.o: %.c
	$(CC) -fno-stack-protector -zexecstack -fno-builtin -mpreferred-stack-boundary=4 -c $<
	@echo "CC <= $<"

clean:
	$(RM) vulnerable_server $(OBJS)
