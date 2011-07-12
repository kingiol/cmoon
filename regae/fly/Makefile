BASEDIR = ../../
include $(BASEDIR)Make.env
LOCALBASE = ../
include $(LOCALBASE)Make.env

SUBDIR = js css

all: $(SUBDIR)
	@$(MULTIMAKE) $(SUBDIR)

clean:
	@$(MULTIMAKE) -m clean $(SUBDIR)

install:
