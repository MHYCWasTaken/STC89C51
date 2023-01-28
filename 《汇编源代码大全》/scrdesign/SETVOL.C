/*
**  SETVOL.C - set, change, or kill a disk volume label
**
**  public domain demo by Bob Stout
**  DOS 5 enhancements suggested by Keith Beedle
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>

#define NUL '\0'

#if defined(__TURBOC__)
 #pragma option -a-
 #include <dir.h>
 #define _dos_findfirst(f,a,b) findfirst(f,b,a)
 #define _dos_findnext(b) findnext(b)
 #define find_t ffblk
 #define _A_VOLID FA_LABEL
 #define attrib ff_attrib
 #define name ff_name
 #define size ff_size
 #define wr_time ff_time
 #define wr_date ff_date
 #define dos_creat _creat
#else
 #include <direct.h>
 #if defined(__ZTC__)
  #pragma ZTC align 1
 #else /* MSC/QC/WATCOM/METAWARE */
  #pragma pack(1)
  int dos_creat(const char *fname, unsigned attrib)
  {
        int fd;

        if (_dos_creat(fname, attrib, &fd))
                return -1;
        else    return fd;
  }
 #endif
 struct fcb {
         char   fcb_drive;
         char   fcb_name[8];
         char   fcb_ext[3];
         short  fcb_curblk;
         short  fcb_recsize;
         long   fcb_filsize;
         short  fcb_date;
         char   fcb_resv[10];
         char   fcb_currec;
         long   fcb_random;
 };

 struct xfcb {
         char           xfcb_flag;
         char           xfcb_resv[5];
         char           xfcb_attr;
         struct fcb     xfcb_fcb;
 };
#endif

#include "dos5boot.h"   /* SNIPPETS file with DOS 5 boot record structure  */

/*
**  NOTE: The following use functions in two other SNIPPETS files,
**        ABSDISK.ASM & ABSDISKC.C
*/

int AbsDiskRead(unsigned short, size_t, size_t, void *);
int AbsDiskWrite(unsigned short, size_t, size_t, void *);

/*
**  Erase an existing volume label
*/

void vol_kill(char *fname)
{
      union REGS regs;
      struct SREGS sregs;
      struct xfcb buf;

      /* Parse the filename into an FCB               */

      segread(&sregs);
      regs.h.ah = 0x29;
      regs.h.al = 0;
      regs.x.si = (unsigned)fname;
      regs.x.di = (unsigned)&buf.xfcb_fcb;
      sregs.es  = sregs.ds;
      intdosx(&regs, &regs, &sregs);

      /* Volume labels require extended FCB's         */

      buf.xfcb_flag = 0xff;
      buf.xfcb_attr = _A_VOLID;

      /* Delete the old label                         */

      regs.h.ah = 0x13;
      regs.x.dx = (unsigned)&buf;
      intdos(&regs, &regs);
}

/*
**  Create a new volume label
*/

void setvol(char *label)
{
      int fd;
      char new_label[13];     /* name + ext + '.' + NUL       */
      struct find_t finfo;
      union REGS regs;

      /*
      **  Change to root directory.
      **
      **  NOTE: To make this more robust, use pushdir() & popdir(),
      **  also from SNIPPETS.
      */
      
      chdir("\\");

      /* If drive is already labeled, remove it               */

      if (0 == _dos_findfirst("*.*", _A_VOLID, &finfo)) do
      {
            if (_A_VOLID & finfo.attrib)
                  break;
      } while (0 == _dos_findnext(&finfo));

      if (_A_VOLID & finfo.attrib)
            vol_kill(finfo.name);

      strcpy(new_label, label);
      if (8 < strlen(label))
      {
            new_label[8] = '.';
            strcpy(&new_label[9], &label[8]);
      }

      fd = dos_creat(new_label, _A_VOLID);/* Create new label */
      close(fd);

      /*
      **  For DOS 5.0 replace the boot record too.
      */

      if(_osmajor > 3)
      {
            int index;
            B_REC boot_record;

            AbsDiskRead(0, 1, 0, &boot_record);
            if(0 == strcmp(boot_record.bsOemName, "MSDOS5.0"))
            {
                  index = 0;
                  while (NUL != label[index])
                  {
                        boot_record.bsVolumeLabel[index] = label[index];
                        index++;
                  }
                  for(index; index < 11; index++)
                        boot_record.bsVolumeLabel[index] = 0x20;
                  AbsDiskWrite(0, 1, 0, &boot_record);
            }
      }
      /*
      **  NOTE: If you used pushdir() above, use popdir() here.
      */
}

#ifdef TEST

void main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("\aUsage: SETVOL new_name");
            abort();
      }
      setvol(argv[1]);
}

#endif
