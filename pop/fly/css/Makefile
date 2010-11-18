BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css \
		b/client/lcs_ie6.css b/client/lcs.css \
		b/index.css b/manual.css \
		b/blog.css b/blogindex.css b/blogop.css \
		b/oms.css b/oms/edit.css b/oms/stat.css b/oms/users.css b/oms/faq.css \
	    b/okol.css
all: preload $(FILES)

preload:
	@mkdir -p b/oms/
	@mkdir -p b/client/

b/base.css: reset.css base.css mid.css overlay.css

b/client/lcs_ie6.css: client/lcs_ie6.css
b/client/lcs.css: client/lcs.css

b/index.css: index.css
b/manual.css: manual.css
b/blog.css: blog/blog.css
b/blogindex.css: blog/index.css
b/blogop.css: blog/mku_skin.css blog/mku_set.css blog/op.css

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
