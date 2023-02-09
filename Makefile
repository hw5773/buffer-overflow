SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: server client

server: server.o common.o
	gcc -o $@ $^

client: client.o common.o
	gcc -o $@ $^

%.o: %.c
	$(CC) -c $< -fno-stack-protector
	@echo "CC <= $<"

clean:
	$(RM) server client $(OBJS) 
