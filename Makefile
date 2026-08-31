CC = g++
SOURCES = main.cpp
EXECUTABLE = total_area

all: ${EXECUTABLE}

${EXECUTABLE}: ${SOURCES}
	${CC} ${SOURCES} -o $@

run:
	./${EXECUTABLE} TotalAreaVTBL4

clean:
	rm ${EXECUTABLE}

