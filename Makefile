CC = g++
EXECUTABLE = total_area
EXEC2 = corner_area
CFLAGS = -O3
HEADERS = shapes.h common.h total_area.h corner_area.h

all: ${EXEC2}

${EXECUTABLE}: main.cpp ${HEADERS}
	${CC} -o $@ ${CFLAGS} main.cpp

${EXEC2}: main_corner.cpp ${HEADERS}
	${CC} -o $@ ${CFLAGS} main_corner.cpp

run:
	./${EXECUTABLE} TotalAreaVTBL4

clean:
	rm ${EXECUTABLE} ${EXEC2}

