

.PHONY: compile
compile: build
	cd build && ninja

build: build/build.tag

.PHONY: run
run: compile
	cd build && bin/cent

.PHONY: debug
debug: compile
	cd build && gdb bin/cent

.PHONY: test
test: build
	cd build && meson test

build/build.tag: conan/conanbuild.sh
	meson setup --native-file conan/conan_meson_native.ini build
	touch build/build.tag

conan/conanbuild.sh: conanfile.txt
	conan install --build=missing -of conan .

.PHONY: clean
clean: clean-build clean-conan

.PHONY: clean-build
clean-build:
	rm -rf build

.PHONY: clean-conan
clean-conan:
	rm -rf conan
