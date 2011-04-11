BASEDIR = ../
include ../Make.env

SUBDIR = pub walk run pager fly mtls

all: $(SUBDIR)
	@$(MULTIMAKE) $(SUBDIR)

clean:
	@$(MULTIMAKE) -m clean $(SUBDIR)

install:
	@mkdir -p -m 775 $(SITE_ROOT)
	@cp config.hdf $(SITE_ROOT)/
	@$(MULTIMAKE) -m install $(SUBDIR)

backup:
	@$(BACKUPDIR) $(SUBDIR)
