BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.js

all: preload $(FILES)

preload:
	@mkdir -p b/

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js \
	       pub/bmoon.js \
		   bmoon.voov.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
