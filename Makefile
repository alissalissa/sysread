#Overarching Makefile

source_dir=bin/

default: all

all:
	$(MAKE) -C $(source_dir)

clean:
	$(MAKE) -C $(source_dir) clean
#	rm ./bin/*.o ./bin/*.so