BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css \
		b/chat_ie6.css b/chat.css \
		b/index.css b/manual.css \
		b/blog.css b/blogindex.css b/blogop.css \
		b/oms.css b/oms/edit.css b/oms/stat.css b/oms/users.css b/oms/faq.css \
	    b/okol.css
all: preload $(FILES)

preload:
	@mkdir -p b/oms/

b/base.css: reset.css base.css mid.css overlay.css

b/chat_ie6.css: chat_ie6.css
b/chat.css: chat.css

b/index.css: index.css
b/manual.css: manual.css
b/blog.css: blog/blog.css
b/blogindex.css: blog/index.css
b/blogop.css: blog/op.css

b/oms.css: oms/oms.css oms/scroll.css
b/oms/edit.css: oms/tabs.css oms/edit.css
b/oms/stat.css: oms/stat.css
b/oms/users.css: oms/users.css oms/overlay.css
b/oms/faq.css: oms/faq.css
b/okol.css: okol/datePicker.css okol/okol.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
