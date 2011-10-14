BASEDIR = ../../../
include $(BASEDIR)Make.env

FILES = b/base.css
all: $(FILES)

b/base.css: index.css flowtools.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
