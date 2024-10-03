
SANITIZE_ADDRESS ?= 0


MESON_OPTS = -Dbuildtype=debug --debug -Ddefault_library=static

ifeq ($(SANITIZE_ADDRESS), 1)
	MESON_OPTS += -Db_sanitize=address
endif

.PHONY: compile
compile: build
	cd build && ninja

build:
	meson setup $(MESON_OPTS) build

TEST_NAME ?= *
.PHONY: test
test:
	cd build && meson test "$(TEST_NAME)"

.PHONY: clean
clean:
	rm -rf build


