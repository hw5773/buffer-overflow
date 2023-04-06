SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: vulnerable_server

vulnerable_server: vulnerable_server.o
	gcc -o $@ $^ -fno-stack-protector

%.o: %.c
	$(CC) -c $< -fno-stack-protector
	@echo "CC <= $<"

clean:
	$(RM) vulnerable_server $(OBJS)
