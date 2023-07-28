
.PHONY: all
all:
	cargo b

.PHONY: format-fix
format-fix:
	 rustfmt $(shell find src -type f -name "*.rs")
