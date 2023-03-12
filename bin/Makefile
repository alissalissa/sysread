#Build the sysfile library
default: compile

#compile
compile: sfutil.o sysheader.o sysvar.o
	$(info Linking shared library...)
	cc -o sysfile.so -g sysvar.o sfutil.o sysheader.o -shared -fPIC

sfutil.o:
	$(info Compiling sfutil.c...)
	cc -g -c -o sfutil.o -I../include/ ../src/sfutil.c

#sysfile.o:
#	echo "Compiling sysfile.c..."
#	cc -g -c -o sysfile.o -I../include/ ../src/sysfile.c

sysheader.o:
	$(info Compiling sysheader.c...)
	cc -g -c -o sysheader.o -I../include/ ../src/sysheader.c

sysvar.o:
	$(info Compiling sysvar.c...)
	cc -g -c -o sysvar.o -I../include/ ../src/sysvar.c