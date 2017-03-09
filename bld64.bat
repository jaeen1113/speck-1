@echo off
yasm -DBIN -fbin spk128.asm -ospk128.bin
yasm -fwin64 spk128.asm -ospk128.obj
cl /nologo test.c spk128.obj speck64.c
del *.obj