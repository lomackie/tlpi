include ../Makefile.inc

all : ${TLPI_LIB}

OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c))

${TLPI_LIB} : ${OBJECTS}
	${RM} ${TLPI_LIB}
	${AR} rs ${TLPI_LIB} *.o

error_functions.o : ename.c.inc

ename.c.inc :
	sh Build_ename.sh > ename.c.inc
	echo 1>&2 "ename.c.inc built"

clean :
	${RM} *.o ename.c.inc ${TLPI_LIB}
