#Build the sysfile library
default: compile

#compile
compile: dfvar.o lsmissing.o lsvlabel.o syschare.o vlsr.o lvnr.o disppar.o epir.o bstream.o mrset.o sfutil.o sysheader.o sysvar.o sysvl.o sysdoc.o sysiinfo.o sysfinfo.o svset.o
	$(info Linking shared library...)
	gcc -o libsysfile.so -g dfvar.o lsmissing.o lsvlabel.o syschare.o vlsr.o lvnr.o disppar.o epir.o bstream.o mrset.o sysiinfo.o sysdoc.o sysvl.o sysvar.o sfutil.o sysheader.o sysfinfo.o svset.o -shared -fPIC

sfutil.o:
	$(info Compiling sfutil.c...)
	gcc -g -c -o sfutil.o -I../include/ ../src/sfutil.c -fPIC

sysvl.o:
	$(info Compiling sysvl.c...)
	gcc -g -c -o sysvl.o -I../include/ ../src/sysvl.c -fPIC

sysheader.o:
	$(info Compiling sysheader.c...)
	gcc -g -c -o sysheader.o -I../include/ ../src/sysheader.c -fPIC

sysvar.o:
	$(info Compiling sysvar.c...)
	gcc -g -c -o sysvar.o -I../include/ ../src/sysvar.c -fPIC

sysdoc.o:
	$(info Compiling sysdoc.c...)
	gcc -g -c -o sysdoc.o -I../include/ ../src/sysdoc.c -fPIC

sysiinfo.o:
	$(info Compiling sysiinfo.c...)
	gcc -g -c -o sysiinfo.o -I../include/ ../src/sysiinfo.c -fPIC

sysfinfo.o:
	$(info Compiling sysfindo.c...)
	gcc -g -c -o sysfinfo.o -I../include/ ../src/sysfinfo.c -fPIC

mrset.o:
	$(info Compiling mrset.c...)
	gcc -g -c -o mrset.o -I../include/ ../src/mrset.c -fPIC

bstream.o:
	$(info Compiling bstream.c...)
	gcc -g -c -o bstream.o -I../include ../src/bstream.c -fPIC

epir.o:
	$(info Compiling epir.c...)
	gcc -g -c -o epir.o -I../include ../src/epir.c -fPIC

disppar.o:
	$(info Compiling disppar.c...)
	gcc -g -c -o disppar.o -I../include ../src/disppar.c -fPIC

svset.o:
	$(info Compiling svset.c...)
	gcc -g -c -o svset.o -I../include ../src/svset.c -fPIC

lvnr.o:
	$(info Compiling lvnr.c...)
	gcc -g -c -o lvnr.o -I../include ../src/lvnr.c -fPIC

vlsr.o:
	$(info Compiling vlsr.c....)
	gcc -g -c -o vlsr.o -I../include ../src/vlsr.c -fPIC

syschare.o:
	$(info Compiling syschare.c....)
	gcc -g -c -o syschare.o -I../include ../src/syschare.c -fPIC

lsvlabel.o:
	$(info Compiling lsvlabel.c....)
	gcc -g -c -o lsvlabel.o -I../include ../src/lsvlabel.c -fPIC

lsmissing.o:
	$(info Compiling lsmissing.c....)
	gcc -g -c -o lsmissing.o -I../include ../src/lsmissing.c -fPIC

dfvar.o:
	$(info Compiling dfvar.c....)
	gcc -g -c -o dfvar.o -I../include ../src/dfvar.c -fPIC

clean:
	rm *.o *.so
