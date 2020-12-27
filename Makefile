CHALLENGE=ch31
USER=app-systeme-$(CHALLENGE)
USER_CRACKED=$(USER)-cracked

CC=gcc
CFLAGS=-no-pie -m32 -g -fno-stack-protector
LDFLAGS=-z execstack

SRC=$(CHALLENGE).c
OBJ=$(SRC:.c=.o)
BIN=$(CHALLENGE)

.DEFAULT_GOAL := challenge
.PHONY : clean all

$(BIN): 
	@echo "Compiling..."
	$(CC) -o $@ bot.c ch31.c log.c msg.c network.c $(LDFLAGS) $(CFLAGS)

challenge: $(BIN)
	@echo "Applying permissions..."
	rm -f $(OBJ)
	chown $(USER_CRACKED):$(USER) $(BIN) .passwd Makefile $(SRC) *.c *.h README
	chmod 400 .passwd
	chmod 440 $(SRC) Makefile
	chmod 550 $(BIN)

