
.PHONY: compile
compile: build/tag
	cd build && ninja


TEST_NAME ?= *

.PHONY: test
test: build/tag
	cd build && meson test "$(TEST_NAME)"

build/tag:
	meson setup build
	touch build/tag
