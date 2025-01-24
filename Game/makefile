game:
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -G Ninja
	cmake --build build

run:
	./build/Game

clean:
	rm -rf build/