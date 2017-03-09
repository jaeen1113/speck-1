@echo off
yasm -DBIN -fbin spk.asm -ospk.bin
yasm -fwin32 spk.asm -ospk.obj
cl /nologo /GS- /O2 /Os /Fa /c speck64.c
jwasm -nologo -bin speck64.asm
cl /nologo test.c speck64.obj spk.obj
del *.obj