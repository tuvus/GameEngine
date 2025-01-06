run: buildgame
	cd Game && make run && cd ..

buildgame:
	cd Game && make && cd ..

test:
	cd GameEngine && make test && cd ..


clean:
	cd GameEngine && make clean && cd ../Game && make clean && cd ..
