/*
** DosDate macros - access bitfield values from DOS file date & time
**  for TC/TC++ by: Wayne Hamilton
*/

#define dd_yr(d)      (((struct { unsigned day:5, mo:4, yr:7; } *) &d)->yr)
#define dd_mo(d)      (((struct { unsigned day:5, mo:4, yr:7; } *) &d)->mo)
#define dd_day(d)     (((struct { unsigned day:5, mo:4, yr:7; } *) &d)->day)
#define dd_hr(t)      (((struct { unsigned sec:5, mn:6, hr:5; } *) &t)->hr)
#define dd_mn(t)      (((struct { unsigned sec:5, mn:6, hr:5; } *) &t)->mn)
#define dd_sec(t)     (((struct { unsigned sec:5, mn:6, hr:5; } *) &t)->sec)

/* then: year = dd_yr(ffblk.ff_date); */
