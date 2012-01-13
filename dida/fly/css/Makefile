LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.css \
        b/index.css \
        b/readme.css \
        b/admin/spd/post.css b/admin/spd/robot.css

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/

b/base.css: reset.css base.css layout.css

b/index.css: index/dateinput.css index/index.css
b/readme.css: readme.css

b/admin/spd/post.css: admin/spd/post.css
b/admin/spd/robot.css: admin/spd/robot.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
