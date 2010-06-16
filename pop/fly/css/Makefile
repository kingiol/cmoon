BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css b/chat.css b/oms.css
all: $(FILES)

b/base.css: reset.css base.css mid.css pub/flowtools/css/overlay-minimal.css
b/chat.css: chat.css
b/oms.css: oms.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
