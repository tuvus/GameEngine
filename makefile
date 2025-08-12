.PHONY: help


## Self-documenting makefile code thanks to https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
help: ## Shows the available makefile targets
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

buildgame: ## Builds the game
	cd Game && make && cd ..

run: buildgame ## build and run the game
	cd Game && make run && cd ..

debug: ## Builds the game with debug flags and address sanitizer
	cd Game && make debug && cd ..

run-debug: debug ## build and run the game in debug mode
	cd Game && make run && cd ..

test: ## Runs the GameEngine tests
	cd GameEngine && make test && cd ..

clean: ## Cleans the build files from both the game engine and game
	cd GameEngine && make clean && cd ../Game && make clean && cd ..
