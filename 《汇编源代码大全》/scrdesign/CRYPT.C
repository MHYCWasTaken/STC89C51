/****************************************************************/
/*                                                              */
/*      S-CODER - Encrypt/decrypt data                          */
/*                                                              */
/*      Copyright 1987-1989 by Robert B. Stout dba MicroFirm    */
/*                                                              */
/*      Originally written by Bob Stout with modifications      */
/*      suggested by Mike Smedley.                              */
/*                                                              */
/*      This code may be used freely in any program for any     */
/*      application, personal or commercial.                    */
/*                                                              */
/*  Current commercial availability:                            */
/*                                                              */
/*      1. MicroFirm Toolkit ver 3.00: LYNX and CRYPT utilities */
/*      2. CXL libraries (MSC, TC, ZTC/C++, PC): fcrypt()       */
/*         dedicated file encryption function                   */
/*      3. SMTC & MFLZT libraries: crypt() function             */
/*                                                              */
/****************************************************************/
 
char *cryptext;         /* The actual encryption/decryption key */
int   crypt_ptr = 0;    /* Circular pointer to elements of key  */
int   crypt_length;     /* Set externally to strlen(cryptext)   */
 
/* NOTES: cryptext should be set and qualified (to something over
          5-6 chars, minimum) by the calling program, which should
          also set crypt_ptr in the range of 0 to strlen(cryptext)
          before each use. If crypt() is used to encrypt several
          buffers, cryptext should be reloaded and crypt_ptr reset
          before each buffer is encrypted. The encryption is both
          reversible - to decrypt data, pass it back through crypt()
          using the original key and original initial value of
          crypt_ptr - and multiple passes are commutative.
*/
 
/**** Encrypt/decrypt buffer datum ******************************/
void crypt(char *buf)
{
        *buf ^= cryptext[crypt_ptr] ^ (cryptext[0] * crypt_ptr);
        cryptext[crypt_ptr] += ((crypt_ptr < (crypt_length - 1)) ?
                cryptext[crypt_ptr + 1] : cryptext[0]);
        if (!cryptext[crypt_ptr])
                cryptext[crypt_ptr] += 1;
        if (++crypt_ptr >= crypt_length)
                crypt_ptr = 0;
}
