
TARGET=lash
CSOURCES = $(shell echo *.c)
OBJECTS = $(CSOURCES:.c=_c.o)

LDFLAGS = -L$(MYOS_PATH)/lib
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	
	$(CC) $(LDFLAGS) $(FLAGS) -o $(TARGET) $(OBJECTS) -lmy 
	
	

include ../base.mk
