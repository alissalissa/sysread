#Build the sysfile library
default: compile

#compile
compile: mrset.o sfutil.o sysheader.o sysvar.o sysvl.o sysdoc.o sysiinfo.o sysfinfo.o
	$(info Linking shared library...)
	gcc -o libsysfile.so -g mrset.o sysiinfo.o sysdoc.o sysvl.o sysvar.o sfutil.o sysheader.o sysfinfo.o -shared -fPIC

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

clean:
	rm *.o *.so
