LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
		b/map.js

all: preload $(FILES)

preload:
	@mkdir -p b/

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js \
           pub/bmoon.js \
           bmoon.dida.js

b/map.js: map.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
