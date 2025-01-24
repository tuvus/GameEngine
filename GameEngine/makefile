gameengine:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -G Ninja
	cmake --build build

test: gameengine
	./build/tests/Test

clean:
	rm -rf build/