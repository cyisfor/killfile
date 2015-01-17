#CFLAGS=-g
PREFIX?=/usr/local/bin

ifneq ($(UID),0)
noroot:=@echo
seteflag:=@echo set -e
gohere:=@echo cd $(PWD)
endif

killfile: main.o signalmap.o
	gcc $(CFLAGS) -o $@ $^

install: $(PREFIX)/killfile

signalmap.c: signalmap.py
	python $< > $@.temp
	mv $@.temp $@

clean:
	rm -f *.o killfile signalmap.c

$(PREFIX)/killfile: killfile
	$(gohere)
	$(noroot) strip $<
	$(noroot) install $< $@
