game:
	cmake -B build -G Ninja -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
	cmake --build build

run:
	./build/Game

clean:
	rm -rf build/
