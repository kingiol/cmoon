BASEDIR = ../../../
include $(BASEDIR)Make.env

all: b/base.css b/chat.css

b/base.css: reset.css base.css mid.css
b/chat.css: chat.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f b/base.css b/chat.css
