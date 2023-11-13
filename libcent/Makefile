
.PHONY: dev
dev: build/dev/tag
	cd build/dev && ninja

.PHONY: prod
prod: build/prod/tag
	cd build/prod && ninja

TEST_NAME ?= *

.PHONY: test
test: build/dev/tag
	cd build/dev && meson test "$(TEST_NAME)"

.PHONY: coverage-html
coverage-html:
	cd build/dev && ninja coverage-html


.PHONY: clean
clean:
	rm -rf build

build/dev/tag:
	meson setup -Db_coverage=true -Db_sanitize=address -Db_lundef=false --buildtype=debug build/dev
	touch build/dev/tag

build/prod/tag:
	meson setup --buildtype=release build/prod
	touch build/prod/tag

