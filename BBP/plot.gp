set title 'Wall-clock time - using clock\_gettime()'
set xlabel 'N'
set ylabel 'Time (sec)'
set terminal png font " Times_New_Roman,12 "
set key left
set output 'runtime.png'

plot \
"out.txt" using 1:2 smooth csplines lw 2 title 'Original', \
"out.txt" using 1:3 smooth csplines lw 2 title 'OpenMP (2 threads)', \
"out.txt" using 1:4 smooth csplines lw 2 title 'OpenMP (4 threads)'
