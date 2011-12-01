LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.css \
		b/icar.css \
		b/admin/spd/post.css

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/

b/base.css: reset.css base.css layout.css

b/icar.css: icar/dateinput.css icar/icar.css

b/admin/spd/post.css: admin/spd/post.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
