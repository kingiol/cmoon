BASEDIR = ../../../
include $(BASEDIR)Make.env

.PHONY: pub
all: pub

pub:
	@$(MAKE) -C pub

lbase.js: local/bmoon.lcs.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@$(MAKE) -C pub clean
	@rm -f lbase.js
