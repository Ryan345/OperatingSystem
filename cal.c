#include "blackdos.h"

int firstOfMonth(int,int);
int isLeapYear(int);

void main()
{
   int mo, day, yr, noDays, i, c;
   mo = -1;
   while (mo < 1 || mo > 12)
   {
      PRINTS("Enter desired month: \0");
      SCANN(mo);
   }
   PRINTS("Enter desired year: \0");
   SCANN(yr);
   if (yr < 100) yr += 2000;
   day = firstOfMonth(mo,yr);
   PRINTS("\r\n\0");
   switch(mo)
   {
      case 1:  PRINTS("January \0");   noDays = 31; break;
      case 2:  PRINTS("February \0");  noDays = 28 + isLeapYear(yr); break;
      case 3:  PRINTS("March \0");     noDays = 31; break;
      case 4:  PRINTS("April \0");     noDays = 30; break;
      case 5:  PRINTS("May \0");       noDays = 31; break;
      case 6:  PRINTS("June \0");      noDays = 30; break;
      case 7:  PRINTS("July \0");      noDays = 31; break;
      case 8:  PRINTS("August \0");    noDays = 31; break;
      case 9:  PRINTS("September \0"); noDays = 30; break;
      case 10: PRINTS("October \0");   noDays = 31; break;
      case 11: PRINTS("November \0");  noDays = 30; break;
      default: PRINTS("December \0");  noDays = 31;
   }
   PRINTN(yr);
   PRINTS("\r\n\0");
   PRINTS("Su Mo Tu We Th Fr Sa\r\n\0");
   PRINTS("====================\r\n\0");
   for (i = 1; i <= day; i++) PRINTS("   \0");
   c = day;
   for (i = 1; i <= noDays; i++)
   {
      if (i < 10) PRINTS(" \0");
      PRINTN(i);
      PRINTS(" \0");
      c = c + 1;
      if (c == 7)
      {
         c = 0; PRINTS("\r\n\0");
      }
   }
   PRINTS("\r\n\0");
   END;
}

int mod(int a, int b)
{
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

int firstOfMonth(int m, int y)
{
   int a, b, c, e, f, g, h, w, z;
   a = (14 - m) / 12;
   b = y - a;
   c = b / 4;
   e = b / 100;
   f = b / 400;
   g = m + 12 * a - 2;
   h = (31 * g) / 12;
   w = b + c + 1 - e + f + h;
   z = mod(w,7);
   return z;
}

int isLeapYear(int y)
{
   int a, b, c;
   a = mod(y,4);
   b = mod(y,100);
   c = mod(y,400);
   if ((a == 0) && ((b != 0) || (c == 0)))
      return 1;
   else return 0;
}
