
	
FLAGS = -g

## TODO:
ERL_ROOT = /usr/local/lib/erlang/erts-5.10.4

ECC = erlc

OUTDIR = ./
RCS = $(wildcard *.erl)
OBJS = $(patsubst %.erl,$(OUTDIR)/%.beam,$(RCS))

all:aoi_nif.so $(OBJS) test testmap

aoi_nif.so:aoi.c aoi.h aoi_nif.c
	gcc -o $@ $^ --shared -fpic $(FLAGS) -Wall -I $(ERL_ROOT)/emulator/beam -I $(ERL_ROOT)/include
	
%.beam:%.erl
	$(ECC) $^
	
test:
	gcc -o test -g -Wall aoi.c test.c

testmap:
	gcc -o testmap -g -Wall testmap.c

clean: 
	rm  aoi_nif.so *.beam test testmap


	