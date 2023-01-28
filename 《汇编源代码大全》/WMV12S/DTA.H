#define A_FIL	0x40  /* Peter's addition: this mask searches for file */
#define A_ARC	0x20  /* attributes bits of attribute byte in dta */
#define A_DIR	0x10
#define A_SYS	0x4
#define A_HID	0x2
#define A_MASK	(A_FIL | A_DIR | A_SYS | A_HID)  /* mask to find all files */

struct dtbuf3 {  /* structure returned by dos function 0x4e and 0x4f */
  /* The first 21 bytes are undocumented; use them at your own risk.
  ** Their use is guessed by Peter Wu. This applies only to DOS 3.xx
  */

  unsigned char drv_no;      /* drive number; 1=A  2=B	3=C ... */
  char template[11];	     /* file template; no period */
  unsigned char match_attr;  /* the search attribute */
  unsigned char slotl;	     /* directory slot number of the matching file */
  unsigned char sloth;
  unsigned char clusl, clush; /* cluster number of the directory being
				 searched */
  unsigned char unknown[4];   /* haven't figured out what these are */
  /* end of first 21 bytes */

  unsigned char attr;
  unsigned short time;
  unsigned short data;
  unsigned long size;  /* 4 bytes */
  char fn[13];	/* this is an asciiz */
  unsigned char e_attr;  /* extended (by peter) search attribute */
};

struct dtbuf2 {  /* structure returned by dos function 0x4e and 0x4f */
  /* The first 21 bytes are undocumented; use them at your own risk.
  ** Their use is guessed by Peter Wu. This applies only to DOS 2.xx
  */

  unsigned char match_attr;  /* the search attribute */
  unsigned char drv_no;      /* drive number; 1=A  2=B	3=C ... */
  char template[11];	     /* file template; no period */
  unsigned char slotl;	     /* directory slot number of the matching file */
  unsigned char sloth;
  unsigned char unknown[4];   /* haven't figured out what these are */
  unsigned char clusl, clush; /* cluster number of the directory being
				 searched */
  /* end of first 21 bytes */

  unsigned char attr;
  unsigned short time;
  unsigned short data;
  unsigned long size;  /* 4 bytes */
  char fn[13];	/* this is an asciiz */
  unsigned char e_attr;  /* extended (by peter) search attribute */
};

struct dtbufx {  /* generic version (works for any DOS version */
  unsigned char unknown[21];
  unsigned char attr;
  unsigned short time;
  unsigned short data;
  unsigned long size;  /* 4 bytes */
  char fn[13];	/* this is an asciiz */
  unsigned char e_attr;  /* extended (by peter) search attribute */

  /* the following info has to be copied from dtbuf2 or dtbuf3 by
  ** the ffmf and fnmf functions
  */
  unsigned char drv_no;
  unsigned char slotl, sloth, clusl, clush;
};

union dtbuf {
  struct dtbuf3 dos3;
  struct dtbuf2 dos2;
  struct dtbufx dos;
};
