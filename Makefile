msvc:
		cl /nologo /O2 /Ot /DTEST test.c speck32.c speck64.c speck128.c
gnu:
		gcc -DTEST -Wall -O2 test.c speck32.c speck64.c speck128.c -otest	 
clang:
		clang -DTEST -Wall -O2 test.c speck32.c speck64.c speck128.c -otest	    