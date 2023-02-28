
CC=gcc
LDFLAGS=-lMLV
EXEC=attaxx
SRC=Attaxx.c

$(EXEC): $(SRC)
	$(CC) -o $@ $< $(LDFLAGS)

clean: 
	rm $(EXEC)