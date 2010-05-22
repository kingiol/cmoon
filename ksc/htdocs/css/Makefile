BASEDIR = ../../../
include $(BASEDIR)Make.env

all: mflowtools.css msuperfish.css

mflowtools.css: pub/ref/flowtools/css/mtabs.css pub/ref/flowtools/css/tooltip.css pub/ref/flowtools/css/expose.css \
			   pub/ref/flowtools/css/overlay.css pub/ref/flowtools/css/scrollable.css

msuperfish.css: pub/ref/superfish/css/superfish.css pub/ref/superfish/css/superfish-vertical.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f mflowtools.css msuperfish.css
