# The gnuplot script for the log watcher data processors.
# Creates a histogram of log events that shows their distribution over the given period.
# The required bins are calculated automatically.
#
# Data format:
# The data is expteced to be found in a file.
# Only the time stamps (single column) are required.
# The column separator is a semicolon (;).
#
# Arguments:
#   file_name       : The data file name and path
#   y_label         : The label of the Y axis

if(!exists("file_name")) exit
if(!exists("y_label")) exit

set key autotitle columnhead
# Number of intervals
n=96
# Get some required stats to calculate the bins
stats file_name nooutput
max=STATS_max
min=STATS_min
width=(max-min)/n
#function used to map a value to the intervals
hist(x,width)=width*floor(x/width)+width/2.0
set boxwidth width*0.9
# General setup
# Note that setting xdata to time will disallow further use of stats command for that axis
set terminal pngcairo size 1000,400 background rgb '#636363'
set ylabel y_label
set datafile separator ";"
set xdata time
set timefmt "%s"
set format x "%H:%M"
set xlabel "Time"
set style fill transparent solid 0.2 noborder
set grid
set xtics rotate
set key noenhance

#count and plot
plot file_name u (hist($1,width)):(1.0) smooth freq w boxes notitle
