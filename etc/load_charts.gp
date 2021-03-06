# The gnuplot script for the load data processors.
#
# Data format:
# The data is expteced to be found in a file.
# X axis data is the first column (index 0), followed by there data columns
# The column separator is a semicolon (;).
#
# Arguments:
#   file_name       : The data file name and path

if(!exists("file_name")) exit

set terminal pngcairo size 1000,400 background rgb '#636363'
set datafile separator ";"
set xdata time
set timefmt "%s"
set format x "%H:%M"
set xlabel "Date"
set ylabel "Load average"
set grid
set xtics rotate
set key noenhanced

plot for [i=2:4] file_name using 1:i with lines title columnheader(i)
