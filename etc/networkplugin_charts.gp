# The gnuplot script for the network usage processors

# Data format:
# The data is expteced to be found in a file.
# X axis data is the first column (index 0), followed by the rx and tx column
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
set ylabel "Network usage [kbyte/s]"
set grid
set xtics rotate
set key noenhanced

plot file_name using 1:($2*0.0009765625) with lines title columnheader(2), \
  file_name using 1:($3*0.0009765625) with lines title columnheader(3),
