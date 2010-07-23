BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css b/chat.css b/chat_ie6.css b/oms.css b/oms/edit.css b/oms/stat.css b/blog.css
all: preload $(FILES)

preload:
	@mkdir -p b/oms/

b/base.css: reset.css base.css mid.css index.css pub/flowtools/css/overlay-minimal.css
#pub/superfish-1.4.8/css/superfish.css
b/chat.css: chat.css
b/chat_ie6.css: chat_ie6.css
b/oms.css: oms/oms.css
b/oms/edit.css: oms/tabs.css oms/edit.css
b/oms/stat.css: oms/stat.css
b/blog.css: blog.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
