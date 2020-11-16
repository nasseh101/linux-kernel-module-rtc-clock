ifeq ($(KERNELRELEASE),)  

KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)  

.PHONY: build clean  

test:
	gcc test.c -o test

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c test
else  

$(info Building with KERNELRELEASE = ${KERNELRELEASE}) 
obj-m :=    clock.o  

endif