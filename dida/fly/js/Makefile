LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
		b/map.js \
		b/spd/58.user.js b/spd/58.js

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/spd/

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js \
           pub/bmoon.js \
           bmoon.dida.js

b/map.js: map.js

b/spd/58.user.js: spd/58.user.js
b/spd/58.js: spd/58.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
