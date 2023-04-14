#Overarching Makefile

source_dir=bin/

default: compile

compile:
	$(MAKE) -C $(source_dir)

clean:
	rm ./bin/*.o ./bin/*.so