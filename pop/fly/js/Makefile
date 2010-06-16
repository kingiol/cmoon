BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.js b/chat.js b/oms.js b/appnew.js
all: $(FILES)

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/jquery.md5.js \
		   pub/jquery.mninpval.js \
	       pub/flowtools/js/tools.overlay-1.1.2.js \
	       pub/bmoon.js bmoon.lcs.js

b/chat.js: pub/jquery.hotkeys.js ape/app/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js
b/oms.js: pub/jquery.hotkeys.js ape/app/bmoon.chatb.js ape/Build/apeClient.js ape/app/livecsb.js
b/appnew.js: pub/jquery.md5.js pub/jquery.mninpval.js app/new.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
