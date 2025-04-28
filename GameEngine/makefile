gameengine:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake DCMAKE_EXPORT_COMPILE_COMMANDS=ON

	cmake --build build

test: gameengine
	./build/tests/Test

clean:
	rm -rf build/
