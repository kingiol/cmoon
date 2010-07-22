BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css b/chat.css b/chat_ie6.css b/oms.css b/blog.css
all: $(FILES)

b/base.css: reset.css base.css mid.css index.css pub/flowtools/css/overlay-minimal.css
#pub/superfish-1.4.8/css/superfish.css
b/chat.css: chat.css
b/chat_ie6.css: chat_ie6.css
b/oms.css: oms.css
b/blog.css: blog.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
