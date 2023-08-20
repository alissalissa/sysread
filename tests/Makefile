#build the unit test framework
default: compile

compile:
	$(info "Building unit test framework...")
	gcc -c -g -o mrset_test.o -I../include  mrset_test.c
	gcc -c -g -o writer.o -I../include writer.c
	gcc -c -g -o epir_test.o -I../include epir_test.c
	gcc -c -g -o disppar_test.o -I../include disppar_test.c
	gcc -c -g -o svtest.o -I../include svtest.c
	gcc -g -o mrset_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o mrset_test.o
	gcc -g -o epir_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o epir_test.o
	gcc -g -o disppar_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o disppar_test.o
	gcc -g -o svtest -L../bin -Wl,-rpath,../bin -lsysfile svtest.o

clean:
	rm *.o *.sys epir_test mrset_test disppar_test svtest