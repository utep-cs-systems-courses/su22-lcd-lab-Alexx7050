COMMON_OBJECTS  = main.o switches.o wdt_handler.o demo.o buzzer.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I./h
LDFLAGS         = -L./lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: proj.elf

#additional rules for files
proj.elf: ${COMMON_OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: proj.elf
	msp430loader.sh $^

clean:
	rm -rf lib h
	rm -f *.o *.elf
