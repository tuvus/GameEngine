game:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
	cmake --build build

test: game
	./build/tests/Test

clean:
	rm -rf build/