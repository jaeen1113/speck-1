@echo off
cl /nologo test.c speck32.c speck64.c speck128.c
del *.obj