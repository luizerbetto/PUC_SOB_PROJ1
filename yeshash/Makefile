	
	obj-m += t.o




all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) m


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
  
