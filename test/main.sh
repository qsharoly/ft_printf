rm test/main.out
gcc -g test/main.c -L. -lftprintf -Iincludes -o test/main.out
./test/main.out
