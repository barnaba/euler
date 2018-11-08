set terminal wxt enhanced noraise

set style fill   solid 1.00 noborder

plot \
  '1v.dat' using 1:3 with lines title 'i',\
  '1v.dat' using 1:2 with lines title "u",\
  '1v.dat' using 1:4 with lines title "E"
pause 10
reread
