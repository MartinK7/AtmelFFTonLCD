GCC=gcc
TARGET=tablegen
CFLAGS=#$(pkg-config cuda-10.1 --cflags)
LIBS=#$(pkg-config cuda-10.1 --libs)

all:
	$(GCC) main.c -o $(TARGET)
clean:
	rm $(TARGET)
run:
	./$(TARGET)
