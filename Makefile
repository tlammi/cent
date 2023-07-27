
.PHONY: all
all:
	cargo b

.PHONY: format-fix
format-fix:
	 rustfmt $(shell find cent-* -type f -name "*.rs")
