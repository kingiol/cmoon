LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
		b/admin/pos.js \
		b/admin/spd/58.user.js b/admin/spd/58.js b/admin/spd/post.js

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js \
           pub/bmoon.js \
           bmoon.dida.js

b/admin/pos.js: admin/pos.js
b/admin/spd/58.user.js: admin/spd/58.user.js
b/admin/spd/58.js: pub/bmoon.js admin/spd/58.js
b/admin/spd/post.js: pub/bmoon.js admin/spd/post.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
