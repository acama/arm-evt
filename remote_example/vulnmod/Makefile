KERNEL_DIR := /lib/modules/$(shell uname -r)/build
                                                                                                           
obj-m := vulnmod.o                                                                                    
PWD := $(shell pwd)                                                                                        

default:                                                                                                   
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

load:
	insmod ./vulnmod.ko

clean:                                                                                                     
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean                                                 
