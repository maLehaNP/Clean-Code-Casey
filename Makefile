CC = g++
EXECUTABLE = total_area
EXEC2 = corner_area
CFLAGS = -O3 -I include -mavx512f
HEADERS = include/shapes.h include/common.h include/total_area.h include/corner_area.h

all: ${EXECUTABLE} ${EXEC2}

${EXECUTABLE}: src/main.cpp ${HEADERS}
	${CC} -o $@ ${CFLAGS} src/main.cpp

${EXEC2}: src/main_corner.cpp ${HEADERS}
	${CC} -o $@ ${CFLAGS} src/main_corner.cpp

run:
	./${EXEC2}

clean:
	rm ${EXECUTABLE} ${EXEC2}

