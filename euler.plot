set terminal wxt enhanced noraise

set style fill   solid 1.00 noborder

plot \
  'plot.dat' using 1:3 with lines title 'i',\
  'plot.dat' using 1:2 with lines title "u",\
  'plot.dat' using 1:4 with lines title "E"
pause 10
reread
