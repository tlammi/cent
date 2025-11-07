
.PHONY: compile run

compile: build
	cd build && meson compile

build:
	meson setup build

run: compile
	cd build && bin/cent
debug: compile
	cd build && gdb bin/cent

test: build
	cd build && meson test
