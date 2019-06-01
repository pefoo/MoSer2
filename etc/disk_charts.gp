# The gnuplot script for the disk data processors.
#
# Data format:
# The data is expteced to be found in a file.
# X axis data is the first column (index 0), followed by a variable amount of y series data columns.
# The column separator is a semicolon (;).
#
# Arguments:
#   file_name       : The data file name and path
#   y_label         : The label of the Y axis
#   y_series_count  : The y series count

if(!exists("file_name")) exit

set terminal pngcairo size 1000,400 background rgb '#636363'
set datafile separator ";"
set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%H:%M"
set xlabel "Date"
set ylabel "Disk [byte]"
set y2label "Utilization [%]"
set y2range [0:100]
set y2tics nomirror
set ytics nomirror
set style fill transparent solid 0.2 noborder
set grid
set xtics rotate
set key noenhanced

plot \
  for [i=2:3] file_name using 1:i with lines axes x1y1 title columnheader(i), \
  file_name using 1:4 with filledcurves y2=0 axes x1y2 title columnheader(4)
