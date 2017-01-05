objects = tokenizer.o regexp.o ttymode.o keyboardin.o cmdhistory.o pipe.o
cflags = -Wall -c -g -std=gnu99

all : CLI calc listf fref

run : clean all
	./CLI


CLI : CLI.o $(objects)
	gcc -Wall -o $@ $< $(objects)

calc : calc.o $(objects)
	gcc -Wall -o $@ $< $(objects)

listf : listf.o $(objects)
	gcc -Wall -o $@ $< $(objects)

fref : fref.o $(objects)
	gcc -Wall -o $@ $< $(objects)


%.o : %.c
	gcc $(cflags) $<

clean:
	rm -f CLI calc listf fref
	rm -f CLI.o
	rm -f calc.o
	rm -f listf.o
	rm -f fref.o
	rm -f $(objects)
