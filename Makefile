SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: server client

server: server.o
	gcc -o $@ $<

client: client.o
	gcc -o $@ $<

%.o: %.c
	$(CC) -c $< $(COMMON_CFLAGS)
	@echo "CC <= $<"

clean:
	$(RM) server client $(OBJS) 
