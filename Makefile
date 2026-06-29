CC      := gcc
CFLAGS  := -Wall -Wextra -O2 $(shell pkg-config --cflags libmodbus)
LDFLAGS := $(shell pkg-config --libs libmodbus) -lpthread -lm

SRC := main.c register_store.c sensor_sim.c alarm.c modbus_tcp.c tcp_server.c
OBJ := $(SRC:.c=.o)
BIN := scada_server

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
