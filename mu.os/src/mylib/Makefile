LDFLAGS = -shared
TARGET = libmy.so
CSOURCES = $(shell echo *.c)
SSOURCES = $(shell echo *.S)

OBJECTS = $(CSOURCES:.c=_c.o)
OBJECTS += $(SSOURCES:.S=_s.o)

BINDIR = $(PREFIX)/lib

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS) -lc   # Link against glibc

include ../base.mk

