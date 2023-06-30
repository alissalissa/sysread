#build the unit test framework
default: compile

compile:
	$(info "Building unit test framework...")
	gcc -c -g -o mrset_test.o -I../include  mrset_test.c
	gcc -c -g -o writer.o -I../include writer.c
	gcc -g -o mrset_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o mrset_test.o

clean:
	rm mrset_test.o mrset_test test.sys