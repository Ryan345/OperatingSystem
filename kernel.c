
﻿/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:______________Ryan Wake______________ Date:__01/28/2016_        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */

void handleInterrupt21(int,int,int,int);

void main()
{
   char buffer[13312];
   int size;
   char line[80];
   int x;
   
   makeInterrupt21();
   interrupt(33,12,2,8,0);   
   
   interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
   interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
   interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0",0,0);
   interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
   interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
   interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
   interrupt(33,0," V. 1.02, C. 2016. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author(s): Ryan Wake.\r\n\r\n\0",0,0);
   
   interrupt(33,0,"Hola mondo.\r\n\0",0,0);
   interrupt(33,0,"Enter a line: \0",0,0);
   interrupt(33,1,line,0,0);
   interrupt(33,0,"\r\nYou typed: \0",0,0);
   interrupt(33,0,line,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"Enter a number: \0",0,0);
   interrupt(33,14,&x,0,0);
   interrupt(33,0,"\r\nYou entered: \0",0,0);
   interrupt(33,13,x,0,0);
   interrupt(33,0,"\r\n\0",0,0);

  /* Uncomment these two to test readSector */
  /* interrupt(33,2,buffer,30,0);*/
  /* interrupt(33,0,buffer,0,0);*/

  /* Uncomment these two to test read file */
  /* interrupt(33,3,"msg\0",buffer,&size);*/
  /* interrupt(33,0,buffer,0,0);*/

 /* Uncomment these two lines to test launchProgram, change fib to desired filename */
   interrupt(33,4,"fib\0",2,0);
   interrupt(33,0,"Error if this executes\r\n\0",0,0);
   while(1);
}

void printString(char* c)
{
   while(*c != '\0')
   {
      interrupt(16,14*256+*c++,0,0,0);
   }
   return;
}

void readString(char* c)
{
   
   int index = 0;

   while(*c != 0xD)
   {
      *c = interrupt(22,0,0,0,0);
      if (*c == 0xD)
      {
         *c++ = '\0';
         return;
      }
      if (*c == 0x8)
      {
         if (index > 0)
         {
            index = index - 1;
            interrupt(16,14*256+*c,0,0,0);
            interrupt(16,14*256+' ',0,0,0);
            interrupt(16,14*256+*c--,0,0,0);
         }
      }
      else
      {
         index = index + 1;
         interrupt(16,14*256+*c++,0,0,0);
      }
   }
   *c++ = '\0';
   return;
}

void clearScreen(int bx, int cx)
{
   int i;
   for(i=0;i<24;i++)
   {
      interrupt(16,14*256+0xD,0,0,0);
   }
   interrupt(16,512,0,0,0);
   if (bx <= 8)
   {
      if(cx <= 16)
      {
         interrupt(16,1536,4096*(bx-1)+256*(cx-1),0,6223);
      }
   }
   return;
}

int mod(int a, int b)
{
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

int div(int a, int b)
{
   int q = 0;
   while (q * b <= a) q++;
   return (q - 1);
}

void writeInt(int x)
{
   char number[6], *d;
   int q = x, r;
   if (x < 1)
   {
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else
   {
      d = number + 5;
      *d = 0x0; d--;
      while (q > 0)
      {
         r = mod(q,10); q = div(q,10);
         *d = r + 48; d--;
      }
      d++;
   }
   printString(d);
}

void readInt(int* number)
{
   char num[6], *d;
   *number = 0;
   readString(num);
   d = num;
   while (*d != '\0')
   {
      *number = (*number * 10) + (*d - 48);
      *d++;
   }
   return;
}

void readSector(char* buffer, int sector)
{
   int relSecNo = 0;
   int headNo = 0;
   int trackNo = 0;
   int cx = 0;
   int dx = 0;

/* Do the math needed to get cx, dx to pass to the interrupt */
   relSecNo = (mod(sector,18)) + 1;
   headNo = mod((div(sector,18)),2);
   trackNo = div(sector,36);
   cx = trackNo * 256 + relSecNo;
   dx = headNo * 256;

   interrupt(19,513,buffer,cx,dx);
}

void error (int bx)
{
   char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];

   errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
   errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
   errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
   errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
   errMsg0[16] = 10; errMsg0[17] = 0;
   errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
   errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
   errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
   errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
   errMsg1[16] = 0;
   errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
   errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
   errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
   errMsg2[12] = 0;
   errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
   errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
   errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
   errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
   errMsg3[16] = 0;

   switch(bx) {
   case 0: printString(errMsg0); break;
   case 1: printString(errMsg1); break;
   case 2: printString(errMsg2); break;
   default: printString(errMsg3);
   }
}

int strcmp(char* str1, char* str2)
{
/* Compare the char arrays index by index to see if they match */
   while(*str1 != '\0')
   {
      if(*str1 != *str2)
         return 0;

      *str1++;
      *str2++;
   }
   return 1;
}

void readFile(char* fname, char* buffer, int* size)
{
   char data[512];
   int index = 0;
   int found;
   *size = 0;

/* Read sector 2 into my array */
   readSector(data,2);

   while(data[index] != 0x0)
   {
/* Check if the passed in file name matches what we are looking for */
      if(strcmp(fname, &data[index]))
      {
         found = index + 6;
         while(data[found] != 0x0)
         {
/* We found the filename in the directory, read it in sector by sector */
            readSector(buffer, data[found]);
            buffer = buffer + 512;
            *size = *size + 512;
            ++found;
         }
         return;
      }
      index = index + 32;
   }
   error(0);
   return;
}

void runProgram(char* name, int segment)
{
   char buffer[13312];
   int size = 0;
   int newSeg;
   int offset;

   newSeg = segment * 4096;
/* Read the file into our buffer */
   readFile(name,buffer,&size);
   while(offset != size)
   {
/* Put the file into memory character by character */
      putInMemory(newSeg, offset, buffer[offset]);
      offset++;
   }
/* Launch the program */
   launchProgram(newSeg);
}

void stop()
{
   while(1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   switch(ax)
   {
      case 0:
         printString(bx);
         break;
      case 1:
         readString(bx);
         break;
      case 2:
         readSector(bx,cx);
         break;
      case 3:
         readFile(bx,cx,dx);
         break;
      case 4:
         runProgram(bx,cx);
         break;
      case 5:
         stop();
         break;
      case 12:
         clearScreen(bx,cx);
         break;
      case 13:
         writeInt(bx);
         break;
      case 14:
         readInt(bx);
         break;
      case 15:
         error(bx);
         break;
      default:
         printString("Invalid value passed to interrupt.");
         break;
   }
   return;
}
