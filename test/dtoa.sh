rm test/dtoa.out
gcc -g test/dtoa.c -Iincludes -L. -lftprintf -o test/dtoa.out
./test/dtoa.out
