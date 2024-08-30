; TANAY AGARWAL 
; 2201CS88
main: ldc 5
	ldc 3
	sub
	brlz else
    ldc 5
    ldc 0
    stnl 0
    br end
else:
    ldc 3
    ldc 0
    stnl 0
	br end
end:
    HALT