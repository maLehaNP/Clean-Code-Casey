CC = gcc
SOURCES = main.c

all: main

main: ${SOURCES}
	${CC} ${SOURCES} -o $@

clean:
	rm main

