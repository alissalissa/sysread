#Build the sysfile library
default: compile

#compile
compile: sfutil.o sysheader.o
	echo "Linking shared library..."
	cc -o sysfile.so -g sfutil.o sysheader.o -shared -fPIC

sfutil.o:
	echo "Compiling sfutil.c..."
	cc -g -c -o sfutil.o -I../include/ ../src/sfutil.c

#sysfile.o:
#	echo "Compiling sysfile.c..."
#	cc -g -c -o sysfile.o -I../include/ ../src/sysfile.c

sysheader.o:
	echo "Compiling sysheader.c..."
	cc -g -c -o sysheader.o -I../include/ ../src/sysheader.c

#sysvar.o:
#	echo "Compiling sysvar.c..."
#	cc -g -c -o sysvar.o -I../include/ ../src/sysvar.c