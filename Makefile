TARGET = Task1
CC = gcc
PLATFORM := $(shell uname)

ifeq ($(PLATFORM),Linux)
	$(TARGET) : main.o Linux.o
		$(CC) main.o Linux.o -o $(TARGET) -lX11

	main.o : main.cpp
		$(CC) -c main.cpp -o main.o

	Linux.o : Linux.cpp
		$(CC) -c Linux.cpp -o Linux.o

	clean :
		rm $(TARGET) *.o
else
	$(TARGET) : main.o Windows.o
		$(CC) main.o Windows.o -o $(TARGET)

	main.o : main.cpp
		$(CC) -c main.cpp -o main.o

	Windows.o : Windows.cpp
		$(CC) -c Windows.cpp -o Windows.o

	clean :
		rm $(TARGET) *.o
endif
