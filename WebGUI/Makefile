PROG = plcgui
SOURCES = $(PROG).c mongoose.c modbus.c webui.c
CFLAGS = -W -Wall -std=c99 -pthread $(CFLAGS_EXTRA)

all: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)


clean:
	rm -rf *.gc* *.dSYM *.obj *.o $(PROG)