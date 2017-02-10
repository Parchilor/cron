include $(ROOT)/config.$(OSTYPE).mk

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
MAIN_OBJS = main.o fs.o
MODIFY_OBJS = modify.o fs.o

all:$(BINARYDIR)/main $(MAIN_OBJS) $(BINARYDIR)/modify $(MODIFY_OBJS)
$(BINARYDIR)/main:$(MAIN_OBJS)
	$(LD) -o $(BINARYDIR)/main $(MAIN_OBJS) $(LDFLAGS)
$(BINARYDIR)/modify:$(MODIFY_OBJS)
	$(LD) -o $(BINARYDIR)/modify $(MODIFY_OBJS) $(LDFLAGS)
	
#sys.o:sys.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@
#main.o:main.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@
#struct.o:struct.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@
#fs.o:fs.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@
#decode.o:decode.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@
#login.o:login.c
#	$(CROSS_COMPILER)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
.PHONY:clean
