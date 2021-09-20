ifneq ($(KERNELRELEASE),)
obj-m   := mymodule.o
else
KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules
install:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules_install
%:
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
endif
