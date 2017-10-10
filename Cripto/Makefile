#Luiz Felipe Zerbetto Masson RA: 15166804
#José Carlos Clausen Neto RA: 15055825
#Danilo Luis Lopes Raymundo Paixão RA: 15051659
#Yessica Melaine Castillo RA: 13054895
#Janaina Mendes Mano Sanches RA: 07270085
#Raissa Furlan Davinho RA: 15032006

obj-m += CryptoModule.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
