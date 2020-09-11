#CC=gcc
CC=/home/dushn/toolchains/tegra-4.8.1-nv/usr/bin/arm-cortex_a15-linux-gnueabi/arm-cortex_a15-linux-gnueabi-gcc
#CFLAGS=-I. -I./include
#$@ - uzima ime targeta
#$^ -uzima imena iz OBJ DIR
#SDIR:= project/$(TARGET)/src
BDIR= ./project/out
COMDEPS = -I./project/common/
SDEPS = functions.h buttons.h buttons_s.h  drawing.h interrupts.h main_header.h testing_tree.h window.h
#DDEPS = functions.h cpu_usage.h devices.h interrupts.h memory_usage.h network_usage.h task_manager.h
#OBJ= $(SDIR)/*.c
LDFLAGS= -lpthread -std=gnu99
LDFLAGS1= -lpthread 
LDFLAGS2= `pkg-config --libs gtk+-3.0` -lm  -lfontconfig
SCFLAGS=-I. -I./project/server/include  
CCFLAGS=-I. -I./project/client/include	
CPKGFLAGS= `pkg-config --cflags gtk+-3.0`
SSDIR= project/server/src
CSDIR= project/client/src 
SOBJ= $(SSDIR)/*.c
COBJ:= $(CSDIR)/*.c

csrc = project/client/src/*.c
obj = $(csrc:.c=.o) 



ODIR= ./project/obj

all:	server_arm server_x86 client


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(SCFLAGS) $(LDFLAGS)


print: $(COBJ)
	lpr -p $?
	touch print
server_x86:$(SOBJ)
	
	gcc  -o $@ $^     $(SCFLAGS)  $(COMDEPS)  $(LDFLAGS1)   
	#gcc  -o $@ $^     $(SCFLAGS) $(CPKGFLAGS) $(LDFLAGS2)
	mv $@ $(BDIR)
server_arm:$(SOBJ)
	$(CC)  -o $@ $^     $(SCFLAGS) $(COMDEPS)  $(LDFLAGS)  
	mv $@ $(BDIR)
client:	$(csrc)
	echo "Executing the object file"
	gcc  -o $@ $^ $(CCFLAGS)  $(COMDEPS) $(CPKGFLAGS) $(LDFLAGS2) 	
	mv $@ $(BDIR)
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o 
	rm -f *~ server client
	rm -f $(BDIR)/*
 


