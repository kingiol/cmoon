BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/kol.js b/chatb.js b/chatc.js \
		b/chat.js b/chat_gb.js \
		b/base.js \
		b/index.js b/manual.js b/appnew.js b/appreset.js b/apppass.js \
		b/blog.js b/blogindex.js b/blogop.js \
		b/oms.js b/oms/edit.js b/oms/stat_ie.js b/oms/stat.js b/oms/users.js \
	    b/okol_ie.js b/okol.js

all: preload $(FILES)

preload:
	@$(MAKE) -C ape/
	@mkdir -p b/oms/

b/kol.js: bmoon.kol.js
b/chatb.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/bmoon.js
b/chatc.js: pub/jquery.cookie.js pub/bmoon.js

# Client [gb gb3212]
b/chat.js: pub/jquerytools/src/tooltip/tooltip.js pub/jquery.hotkeys.js ape/Build/JSON.js ape/app/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js
b/chat_gb.js: pub/jquerytools/src/tooltip/tooltip.js pub/jquery.hotkeys.js ape/Build/JSON.js ape/app/gb/bmoon.chat.js ape/Build/apeClient.js ape/app/livecs.js

# Server
b/base.js: pub/jquery-1.4.2.js pub/jquery.cookie.js pub/jquery.md5.js \
		   pub/jquery.mninpval.js \
		   pub/jquerytools/src/toolbox/toolbox.expose.js \
	       pub/jquerytools/src/overlay/overlay.js \
	       pub/bmoon.js bmoon.lcs.js \
		   pub/jquery.hotkeys.js

b/index.js: pub/jquerytools/src/scrollable/scrollable.js index.js
b/manual.js: manual.js
b/appnew.js: pub/jquery.md5.js pub/jquery.mninpval.js app/new.js
b/appreset.js: app/reset.js
b/apppass.js: app/pass.js
b/blog.js: blog/blog.js
b/blogindex.js: blog/index.js
b/blogop.js: pub/markitup/markitup/jquery.markitup.js blog/mku_set.js blog/op.js

b/oms.js: pub/jquery.mnscrollpage.js ape/Build/JSON.js \
		  ape/app/bmoon.chatb.js ape/Build/apeClient.js ape/app/livecsb.js \
		  pub/jquerytools/src/scrollable/scrollable.js pub/jquerytools/src/scrollable/scrollable.navigator.js \
		  oms/oms.js
b/oms/edit.js: pub/jquerytools/src/tabs/tabs.js oms/edit.js
b/oms/stat_ie.js: pub/datepicker/jquery.bgiframe.min.js pub/flot/excanvas.min.js
b/oms/stat.js: pub/datepicker/date.js pub/datepicker/jquery.datePicker.js \
		       pub/flot/jquery.flot.js \
			   oms/stat.js
b/oms/users.js: app/new.js oms/users.js

b/okol_ie.js: pub/datepicker/jquery.bgiframe.min.js pub/flot/excanvas.min.js
b/okol.js: pub/datepicker/date.js pub/datepicker/jquery.datePicker.js \
	       pub/flot/jquery.flot.js \
	       okol/okol.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
	@$(MAKE) clean -C ape/
