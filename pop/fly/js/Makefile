BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.js b/chat.js b/chatb.js b/chatc.js b/oms.js b/appnew.js
all: preload $(FILES)

preload:
	@$(MAKE) -C ape/

b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/jquery.md5.js \
		   pub/jquery.mninpval.js \
		   pub/jquerytools/src/toolbox/toolbox.expose.js \
	       pub/jquerytools/src/overlay/overlay.js \
	       pub/bmoon.js bmoon.lcs.js

b/chat.js: pub/jquerytools/src/tooltip/tooltip.js pub/jquery.hotkeys.js ape/app/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js
b/chatb.js: pub/jquery-1.4.2.js pub/jquery.cookie.js
b/chatc.js: pub/jquery.cookie.js

b/oms.js: pub/jquery.hotkeys.js ape/app/bmoon.chatb.js ape/Build/apeClient.js ape/app/livecsb.js
b/appnew.js: pub/jquery.md5.js pub/jquery.mninpval.js app/new.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
