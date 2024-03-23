#build the unit test framework
default: compile

compile:
	$(info "Building unit test framework...")
	gcc -c -g -o mrset_test.o -I../include  mrset_test.c
	gcc -c -g -o writer.o -I../include writer.c
	gcc -c -g -o epir_test.o -I../include epir_test.c
	gcc -c -g -o disppar_test.o -I../include disppar_test.c
	gcc -c -g -o svtest.o -I../include svtest.c
	gcc -c -g -o lvnr_test.o -I../include lvnr_test.c
	gcc -c -g -o lsvlabel_test.o -I../include `pkg-config --cflags check` lsvlabel_test.c
	gcc -g -o mrset_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o mrset_test.o
	gcc -g -o epir_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o epir_test.o
	gcc -g -o disppar_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o disppar_test.o
	gcc -g -o svtest -L../bin -Wl,-rpath,../bin -lsysfile svtest.o writer.o
	gcc -g -o lvnr_test -L../bin -Wl,-rpath,../bin -lsysfile writer.o lvnr_test.o `pkg-config --cflags --libs check`
	gcc -g -o header_test -L../bin -Wl,-rpath,../bin -lsysfile -I../include writer.o header_test.c
	gcc -g -o vlsr_test -L../bin -Wl,-rpath,../bin -lsysfile -I../include writer.o vlsr_test.c
	gcc -g -o lsvlabel_test -L../bin -Wl,-rpath,../bin -lsysfile lsvlabel_test.o writer.o `pkg-config --cflags --libs check`

clean:
	rm *.o *.sys epir_test mrset_test disppar_test svtest lsvlabel_test lvnr_test vlsr_test header_test