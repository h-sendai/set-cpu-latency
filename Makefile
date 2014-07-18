PROG = set-cpu-latency
CFLAGS = -g -O2 -Wall
# LDLIBS += -L/usr/local/lib -lmylib

all: $(PROG)
OBJS += $(PROG).o
OBJS += set_timer.o
OBJS += my_signal.o
OBJS += get_num.o

$(PROG): $(OBJS)

clean:
	rm -f *.o $(PROG)
