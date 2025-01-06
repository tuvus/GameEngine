buildgame:
	cd Game && make -f makefile

test:
	cd GameEngine && make test && cd ..


clean:
	cd GameEngine && make clean && cd ..
