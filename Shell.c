#include "blackdos.h"

void main()
{
   char* x;
   PRINTS("cxxxx][===blackdos===> \0");
   SCANN(x);
   switch(x)
   {
      case "boot\0":
         interrupt(33,11,0,0,0);
         break;
   }
   PRINTS("\r\n\0");
   END;
}
