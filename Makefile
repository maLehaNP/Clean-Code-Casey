CC = g++
EXECUTABLE = total_area
CFLAGS = -O3
SOURCES = main.cpp
HEADERS = shapes.h common.h total_area.h

all: ${EXECUTABLE}

${EXECUTABLE}: ${SOURCES} ${HEADERS}
	${CC} -o $@ ${CFLAGS} ${SOURCES}

run:
	./${EXECUTABLE} TotalAreaVTBL4

clean:
	rm ${EXECUTABLE}

