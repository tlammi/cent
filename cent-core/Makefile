
.PHONY: compile
compile: build/tag
	cd build && ninja

.PHONY: test
test: build/tag
	cd build && ninja test

build/tag:
	meson setup build
	touch build/tag
