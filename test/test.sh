rm test/test.out
gcc -g test/main.c -L. -lftprintf -Iincludes -o test/test.out
./test/test.out
