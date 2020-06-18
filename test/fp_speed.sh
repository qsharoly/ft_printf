rm test/fp_speed.out test/fp_speed_std.out
gcc -g test/fp_speed.c -Iincludes -L. -lm -lftprintf -o test/fp_speed.out
gcc -g test/fp_speed_std.c -lm -o test/fp_speed_std.out
echo ft_printf:
time ./test/fp_speed.out > /dev/null
echo "\n\nstdio printf:"
time ./test/fp_speed_std.out > /dev/null
