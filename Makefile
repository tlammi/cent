
.PHONY: all
all:
	cargo b

.PHONY: format-fix
format-fix:
	 rustfmt $(shell find -type f -name "*.rs")
