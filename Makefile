

.PHONY: compile
compile: build
	cd build && ninja

build:
	meson setup build

TEST_NAME ?= *
.PHONY: test
test:
	cd build && meson test "$(TEST_NAME)"

.PHONY: clean
clean:
	rm -rf build


