# Arguments:
#   titletext: The title text for the plot
set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
set title titletext
plot sin(x)
