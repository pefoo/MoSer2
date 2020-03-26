# The gnuplot script for the disk data processors.
#
# Data format:
# The data is expteced to be found in a file.
# X axis data is the first column (index 0), followed by a variable amount of y series data columns.
# The column separator is a semicolon (;).
#
# Arguments:
#   file_name       : The data file name and path
#   title           : The chart title

if(!exists("file_name")) exit
if(!exists("title")) exit

set terminal pngcairo size 1000,400 background rgb '#636363'
set datafile separator ";"
set title title
set xdata time
set timefmt "%s"
set format x "%H:%M"
set xlabel "Date"
set ylabel "Disk [MB]"
set y2label "Utilization [%]"
set y2range [0:100]
set y2tics nomirror
set ytics nomirror
set style fill transparent solid 0.2 noborder
set grid
set xtics rotate
set key noenhanced

plot \
  file_name using 1:($2*0.00000095367432) with lines axes x1y1 title columnheader(2), \
  file_name using 1:($3*0.00000095367432) with lines axes x1y1 title columnheader(3), \
  file_name using 1:4 with filledcurves y2=0 axes x1y2 title columnheader(4)
