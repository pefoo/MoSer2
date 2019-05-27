# The gnuplot script for the cpu data processors.
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
if(!exists("y_label")) exit

set terminal pngcairo size 1000,400 background rgb '#636363'
set datafile separator ";"
set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%H:%M"
set xlabel "Date"
set ylabel y_label
set grid
set xtics rotate

plot for [i=2:y_series_count+1] file_name using 1:i with lines title columnheader(i)
