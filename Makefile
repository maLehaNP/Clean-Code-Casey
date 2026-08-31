CC = g++
SOURCES = main.cpp
EXECUTABLE = total_area

all: ${EXECUTABLE}

${EXECUTABLE}: ${SOURCES}
	${CC} ${SOURCES} -o $@

clean:
	rm main

