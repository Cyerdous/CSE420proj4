CC = gcc
CFLAGS = -Wall
LIBS = 

SRCS = fsa.c
OBJS = $(SRCS:.c=.o)

TARG = fsa

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

%.o:  %.c
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f $(OBJS) $(TARG) *~