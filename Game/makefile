game:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
	cmake --build build

run:
	./build/Game

clean:
	rm -rf build/
