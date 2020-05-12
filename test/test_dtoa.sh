rm test/test_dtoa.out
gcc -g test/test_dtoa.c -L. -lftprintf -Iincludes -o test/test_dtoa.out
./test/test_dtoa.out
