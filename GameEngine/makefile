gameengine:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
	cmake --build build

test: gameengine
	./build/tests/Test

clean:
	rm -rf build/