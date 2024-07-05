SRCS = static_snapshot.c
SRCSC = static_custom_snapshot.c

LIBSRCS = myopen.c myclose.c set.c get.c do_snapshot.c do_recovery.c
LIBSRCSC = custom_open.c myclose.c set.c get.c do_custom_snapshot.c do_custom_recovery.c

TARG = snapshot_static
TARGC = snapshot_custom

CC = gcc
OPTS = -Wall -g -pg
LIBS = -L. -lkvs
LIBSC = -L. -lckvs


OBJS = $(SRCS:.c=.o)
OBJSC = $(SRCSC:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)
LOBJSC = $(LIBSRCSC:.c=.o)
LARS = libkvs.a
LARSC = libckvs.a


all: 

static: $(TARG)

custom: $(TARGC)

$(TARG) : $(OBJS) $(LARS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

$(TARGC) : $(OBJSC) $(LARSC)
	$(CC) -o $(TARGC) $(OBJSC) $(LIBSC)

$(LARS) : $(LOBJS)
	ar rs  $(LARS) $(LOBJS)

$(LARSC) : $(LOBJSC)
	ar rs  $(LARSC) $(LOBJSC)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJSC) $(LOBJS) $(LOBJSC) $(LARS) $(LARSC) $(TARG) $(TARGC)
