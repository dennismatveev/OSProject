dd if=/dev/zero of=diskc.img bs=512 count=256
nasm bootload.asm
dd if=bootload of=diskc.img conv=notrunc

bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o

#dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc


bcc -ansi -c -o tstpr1.o tstpr1.c
as86 -o userlib.o userlib.asm
ld86 -d -o tstpr1 tstpr1.o userlib.o
bcc -ansi -c -o tstpr2.o tstpr2.c
ld86 -d -o tstpr2 tstpr2.o userlib.o

bcc -c -ansi -o shell.o shell.c
ld86 -d -o shell shell.o userlib.o

gcc -o loadFile loadFile.c
./loadFile kernel
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2
./loadFile shell
