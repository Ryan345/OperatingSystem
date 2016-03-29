dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o Shell.o Shell.c
as86 kernel.asm -o kernel_asm.o
as86 lib.asm -o lib_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
ld86 -o Shell -d Shell.o lib_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
chmod +x loadFile.c
./loadFile Shell
