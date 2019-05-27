# Arguments:
#   file_name       : The data file name and path
#   y_series_count  : The y series count

set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%H:%M"
set xlabel "Date"
set datafile separator ";"

plot for [i=2:y_series_count+1] file_name using 1:i with lines title columnheader(i)
