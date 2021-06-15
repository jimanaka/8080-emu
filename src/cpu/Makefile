CC=gcc
CFLAGS= -g -O0 -Wall
TARGET=cpu

$(TARGET): $(TARGET).o
	$(CC) $(TARGET).o -o $(TARGET)

$(TARGET).o: $(TARGET).c
	$(CC) -c $(CFLAGS) $(TARGET).c

clean:
	rm *.o $(TARGET)
