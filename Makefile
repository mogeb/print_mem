# Makefile
#obj-m += stack_tasks.o
obj-m += mem_tasks.o
obj-m += timer_test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

