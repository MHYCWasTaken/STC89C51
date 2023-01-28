SET TALK OFF
mprinter=0
CLEA
LOAD CHECKPR
@1,0 SAY "Printer to check (0-2)" GET mprinter PICT "9" RANGE 0,2
READ
mprinter=LTRI(STR(mprinter))
CALL CHECKPR WITH mprinter
DO CASE
CASE mprinter="O"
     mstat='Printer ready'
CASE mprinter="I"
     mstat='Printer I/O error...printer turned off?'
CASE mprinter="N"
     mstat='Printer not selected'
CASE mprinter="P"
     mstat='Printer out of paper'
CASE mprinter="W"
     mstat='Wrong parameter for CHECKPR or I screwed up <grin>'
CASE mprinter="V"
     mstat='No printer port?'
CASE mprinter="L"
     mstat='Printer offline'
CASE mprinter="C"
     mstat='Printer not connected'
CASE mprinter="U"
     mstat='Unknown problem with printer'
OTHE
     mstat='I definitely screwed up here...CHECKPR choked (whoops)'
ENDC
@3,0 SAY "CHECKPR returns: "+mstat
WAIT
RELE MODU CHECKPR
RETU

