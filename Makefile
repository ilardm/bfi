# $^ - parametre
# $@ - goal

result	=	bfi
incpath =	-I/usr/include/ -Isrc/
src	=	src/main.c
obj	=	main.o

all: release

#------------------

debug: dobj
	gcc -o $(result) $(obj)

release: robj
	gcc -o $(result) $(obj)

#------------------

robj: $(src)
	gcc -O3 -c -D_RELEASE $(src) $(incpath)

dobj: $(src)
	gcc -g -O0 -c -D_DEBUG  $(src) $(incpath)

#------------------

clean:
	rm -f *.o
	rm -f $(result)
