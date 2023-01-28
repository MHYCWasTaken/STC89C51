The following files are included in this set.  They are for:

SCREENS  ARC  -- The "SQZ" files for the included screens
PAS_SRC  ARC  -- The Turbo source and screen for Bindscrn.com
CRSONOF  ASM  -- Source for the cursor routines (from NAN12.ARC)
CRSONOF  OBJ  \
DEMOSCRN OBJ  |- The linkable object modules for the demo program
DEMODRIV OBJ  /  
DEMODRIV PRG  -- Source for the (silly) demo program
BINDSCRN DOC  -- A rough doc file on using Bindscrn

The routines should work equally well with either W'85 or A'86
versions of Clipper.  I've used them with both.

You will need a version of SAYWHAT! that supports the inclusion of 
screens in Turbo Pascal as external procedures.  The programs in this
set use essentially the same approach.  The version I used was dated
4/11/86.  I would guess that any published since then will be OK.

To link the demo:
link demodriv+crsonof+demoscrn;
              or
plink86 FI demodriv,crsonof,demoscrn
