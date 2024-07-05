SRCS = static_main.c
LIBSRCS = myopen.c myclose.c set.c get.c 
TARG = kvs_client

SRCS2 = static_server.c
TARG2 = kvs_server

CC = gcc
LIBS = -L. -lkvs


OBJS = $(SRCS:.c=.o)
OBJS2 = $(SRCS2:.c=.o)

LOBJS = $(LIBSRCS:.c=.o)
LARS = libkvs.a


all: 

static: $(TARG)

server: $(TARG2)

$(TARG) : $(OBJS) 
	$(CC) -o $(TARG) $(OBJS)

$(TARG2) : $(OBJS2) $(LARS)
	$(CC) -o $(TARG2) $(OBJS2) $(LIBS)

$(LARS) : $(LOBJS)
	ar rs $(LARS) $(LOBJS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS2) $(LOBJS) $(LARS) $(TARG) $(TARG2)
