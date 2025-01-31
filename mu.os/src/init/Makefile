
TARGET=init
CSOURCES = $(shell echo *.c)
OBJECTS = $(CSOURCES:.c=_c.o)

LDFLAGS = -L$(MYOS_PATH)/lib
BINDIR = $(PREFIX)/sbin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) -lmy -L/mnt/myos/lib -lc 

include ../base.mk
