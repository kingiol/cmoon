LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
        b/loadgmap.js \
        b/index.js \
        b/admin/pos.js \
        b/admin/spd/58.user.js b/admin/spd/58.js \
        b/admin/spd/ganji.user.js b/admin/spd/ganji.js \
        b/admin/spd/edeng.user.js b/admin/spd/edeng.js \
        b/admin/spd/liebiao.user.js b/admin/spd/liebiao.js \
        b/admin/spd/post.js b/admin/spd/robot.js \
        b/member/regist.js b/member/reset.js b/member/pass.js \
        b/bore/bore.js b/bore/op.js \
        b/plan/info.js

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/
	@mkdir -p b/member/
	@mkdir -p b/bore/
	@mkdir -p b/plan/

b/base.js: pub/jquery-1.7.1.js pub/jquery.cookie.js pub/jquery.md5.js \
           pub/jquery.hotkeys.js pub/jquery.mninpval.js \
           pub/jquerytools/src/toolbox/toolbox.expose.js \
           pub/jquerytools/src/overlay/overlay.js \
           pub/jquery.bmoon.js pub/bmoon.js \
           bmoon.dida.js

b/loadgmap.js: loadgmap.js

b/index.js: pub/jquerytools/src/dateinput/dateinput.js \
            index.js

b/admin/spd/58.user.js: admin/spd/58.user.js
b/admin/spd/58.js: pub/bmoon.js bmoon.dida.js admin/spd/58.js

b/admin/spd/ganji.user.js: admin/spd/ganji.user.js
b/admin/spd/ganji.js: pub/jquery-1.7.1.js pub/jquery.cookie.js \
                      pub/bmoon.js bmoon.dida.js admin/spd/ganji.js

b/admin/spd/edeng.user.js: admin/spd/edeng.user.js
b/admin/spd/edeng.js: pub/jquery-1.7.1.js pub/jquery.cookie.js \
                      pub/bmoon.js bmoon.dida.js admin/spd/edeng.js

b/admin/spd/liebiao.user.js: admin/spd/liebiao.user.js
b/admin/spd/liebiao.js: pub/jquery-1.7.1.js pub/jquery.cookie.js \
                        pub/bmoon.js bmoon.dida.js admin/spd/liebiao.js

b/admin/pos.js: admin/pos.js
b/admin/spd/post.js: admin/spd/post.js
b/admin/spd/robot.js: admin/spd/robot.js

b/member/regist.js: member/regist.js
b/member/reset.js: member/reset.js
b/member/pass.js: member/pass.js

b/bore/bore.js: bore/bore.js
b/bore/op.js: pub/markitup/markitup/jquery.markitup.js bore/mku_set.js \
              pub/jqueryUploader/src/jquery.uploader.js bore/op.js

b/plan/info.js: plan/info.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
