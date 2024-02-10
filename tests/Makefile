#build the unit test framework
default: compile

compile:
	$(info "Building unit test framework...")
	clang -c -g -o mrset_test.o -I../include  mrset_test.c
	clang -c -g -o writer.o -I../include writer.c
	clang -c -g -o epir_test.o -I../include epir_test.c
	clang -c -g -o disppar_test.o -I../include disppar_test.c
	clang -c -g -o svtest.o -I../include svtest.c
	clang -c -g -o lvnr_test.o -I../include lvnr_test.c
	clang -g -o mrset_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o mrset_test.o
	clang -g -o epir_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o epir_test.o
	clang -g -o disppar_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o disppar_test.o
	clang -g -o svtest -L../bin -Wl,-rpath,../bin -lsysfile svtest.o writer.o
	clang -g -o lvnr_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o lvnr_test.o `pkg-config --cflags --libs check`
	clang -g -o header_test -L../bin -Wl,-rpath,../bin -lsysfile -I../include writer.o header_test.c
	clang -g -o vlsr_test -L../bin -Wl,-rpath,../bin -lsysfile -I../include writer.o vlsr_test.c

clean:
	rm *.o *.sys epir_test mrset_test disppar_test svtest