SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: server client server-web

server-web: server-web.o common.o
	gcc -o $@ $^

server: server.o common.o
	gcc -o $@ $^

client: client.o common.o
	gcc -o $@ $^

%.o: %.c
	$(CC) -c $< -fno-stack-protector
	@echo "CC <= $<"

clean:
	$(RM) server client server-web $(OBJS) 
