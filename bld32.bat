@echo off
yasm -DBIN -fbin spk64.asm -ospk64.bin
yasm -fwin32 spk64.asm -ospk64.obj
cl /nologo test.c spk64.obj speck128.c
del *.obj