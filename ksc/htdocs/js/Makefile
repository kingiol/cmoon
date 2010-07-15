BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.js b/prd.js
all: $(FILES)

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/jquery.md5.js \
		   pub/jquery.mninpval.js \
		   pub/jquerytools/src/toolbox/toolbox.expose.js \
		   pub/jquerytools/src/tooltip/tooltip.js \
	       pub/jquerytools/src/overlay/overlay.js \
	       pub/bmoon.js bmoon.ksc.js

b/prd.js: pub/ajaxupload.3.5.js prd_admin.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
