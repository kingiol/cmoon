LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
		b/icar.js \
		b/admin/pos.js \
		b/admin/spd/58.user.js b/admin/spd/58.js b/admin/spd/post.js

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/

b/base.js: pub/jquery-1.7.1.js pub/jquery.cookie.js \
           pub/bmoon.js \
           bmoon.dida.js

b/icar_ie.js: pub/datepicker/jquery.bgiframe.min.js
b/icar.js: pub/datepicker/date.js pub/datepicker/date_zh.js pub/datepicker/jquery.datePicker.js pub/jquerytools/src/tabs/tabs.js icar.js

b/admin/pos.js: admin/pos.js
b/admin/spd/58.user.js: admin/spd/58.user.js
b/admin/spd/58.js: pub/bmoon.js admin/spd/58.js
b/admin/spd/post.js: pub/bmoon.js admin/spd/post.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
