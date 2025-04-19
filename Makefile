CFLAGS = -std=c11 -g -Wall --pedantic -Wshadow -Wvla -Werror -Wunreachable-code
OBJS = main.o tokenizer.o parse_cst.o ast.o
HEADERS = hw13.h
APP = calc

$(APP): $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(APP)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf $(APP)
