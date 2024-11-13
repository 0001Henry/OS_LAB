CC = gcc
CFLAGS = -Wall -g

TARGET = scheduler

SRC = main.c scheduling.c file_io.c queue.c

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
