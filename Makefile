
.PHONY: all
all:
	cargo b

.PHONY: release
release:
	cargo b --release

.PHONY: format-fix
format-fix:
	 rustfmt $(shell find src -type f -name "*.rs" )
