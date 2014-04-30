KERNEL_DIR := /lib/modules/$(shell uname -r)/build
                                                                                                           
obj-m := vulnmod.o                                                                                    
PWD := $(shell pwd)                                                                                        

default:                                                                                                   
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

load:
	insmod ./vulnmod.ko
	mknod /dev/dummyd c 1337 1
	chmod a+rw /dev/dummyd

clean:                                                                                                     
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean                                                 
