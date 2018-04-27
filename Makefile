obj-m := uprinter.o
user_src := uprinter_usr.c
user_dst := uprinter_usr.o
kern:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
user:
	gcc $(user_src) -o $(user_dst)
diag:
	gcc updiag.c -o updiag.o
	gcc picdiag.c -o picdiag.o
insert:
	sudo insmod uprinter.ko
remove:
	sudo rmmod uprinter
run:
	sudo ./uprinter_usr.o
all:
	make kern
	make user
	make diag

