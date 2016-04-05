#include "blackdos.h"

void main()
{
   int i, a = 1, b = 1, c, n;
   PRINTS("\r\nHow many terms? \0");
   SCANN(n);
   PRINTS("\r\n\0");
   if (n < 3) n = 3;
   PRINTN(n);
   PRINTS(" terms: \r\n\0");
   PRINTN(a);
   PRINTS("\r\n\0");
   PRINTN(b);
   PRINTS("\r\n\0");
   for (i = 0; i < n - 2; i++)
   {
      c = a + b;
      PRINTN(c);
      PRINTS(" \r\n\0");
      a = b;
      b = c;
   }
   PRINTS("\r\n\0");
   END;
}
