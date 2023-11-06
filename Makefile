

.PHONY: compile
compile: build/tag
	cd build && ninja

.PHONY: test
test: build/tag
	cd build && ninja test

.PHONY: unity
unity: unity/tag
	cd unity && ninja

.PHONY: clean
clean:
	rm -rf build
	rm -rf unity

build/tag:
	meson setup build
	touch build/tag

unity/tag:
	meson setup -Dbuild_unity=true unity
	touch unity/tag
