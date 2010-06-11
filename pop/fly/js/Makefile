BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.js b/chat.js b/appnew.js
all: $(FILES)

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/bmoon.js bmoon.lcs.js
b/chat.js: pub/jquery.hotkeys.js ape/app/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js
b/appnew.js: pub/jquery.md5.js pub/jquery.mninpval.js app/new.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
