CC = g++
EXECUTABLE = total_area
SOURCES = main.cpp
HEADERS = shapes.h common.h total_area.h

all: ${EXECUTABLE}

${EXECUTABLE}: ${SOURCES} ${HEADERS}
	${CC} ${SOURCES} -o $@

run:
	./${EXECUTABLE} TotalAreaVTBL4

clean:
	rm ${EXECUTABLE}

