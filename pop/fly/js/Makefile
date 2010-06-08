BASEDIR = ../../../
include $(BASEDIR)Make.env

all: b/base.js b/appnew.js

b/base.js: pub/jquery-1.4.2.js pub/bmoon.js bmoon.lcs.js
b/chat.js: ape/app/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js
b/appnew.js: pub/jquery.md5.js pub/jquery.mninpval.js app/new.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f b/base.js b/appnew.js
