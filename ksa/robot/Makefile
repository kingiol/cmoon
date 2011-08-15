LOCALBASE = ../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

INCS	= $(INC_LOCAL) $(INC_MOON)

SOURCES = $(wildcard *.c)
OBJS	= $(patsubst %.c, %.o, $(SOURCES))
DEPEND = .depend

all: librobot.a

$(DEPEND): $(SOURCES)
	@$(CC) $(CFLAGS) -MM $^ $(INCS) > $@

librobot.a:$(OBJS) $(DEPEND)
	@rm -rf $@
	$(AR) rcs $@ $(OBJS)

include $(DEPEND)
%.o:%.c
	@if [ "$<" = `ls *.c|awk '{print $1}'|sed -n '1p'` ]; then \
		echo "=== "$(CFLAGS); \
		echo "=== "$(INCS); fi
	@echo "$(CC) -c $<"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

cflag:
	@echo "=== "$(CFLAGS)
	@echo "=== "$(INCS)

clean:
	@rm -f $(OBJS) *.a
