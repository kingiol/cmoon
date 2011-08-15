LOCALBASE = ./
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

SUBDIR = pub robot cgi pager

all: $(SUBDIR)
	@$(MULTIMAKE) $(SUBDIR)

clean:
	@$(MULTIMAKE) -m clean $(SUBDIR)

backup:
	@$(BACKUPDIR) $(SUBDIR)
