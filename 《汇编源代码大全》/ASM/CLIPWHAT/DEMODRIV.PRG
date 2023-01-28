
clear
external mainscrn, popup1, popup2, main2, main2sub, fast1, fastone,;
         fasttwo, fastthre, fastfour, fastlast, curson, cursoff
do cursoff		&& turn cursor off
do mainscrn
do pause with 1000
save screen
do popup1
do pause with 500
do popup2
do pause with 1500
do main2
@ 22,10 say "Press spacebar"
keypress = inkey(8)
do main2sub
@ 21,10 say "These all had large delays for reading built in.  The screens"
@ 22,10 say "can really pop up quickly (with small delays).  Press a key..." 
keypress = inkey(5)
clear
do mainscrn
do pause with 100
do fast1
do pause with 100
do fastone
do pause with 100
do fasttwo
do pause with 50
do fastthre
do pause with 50
do fastfour
do pause with 25
do fastlast
@ 22,10 say "Press a key to quit."
keypress = inkey(10)
clear
do curson  && restore cursor
quit

procedure pause
parameter counter
for x = 1 to counter
next
return
