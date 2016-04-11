#include "blackdos.h"

void clr();
void help();
void echo(char*);
void loadF(char*,char*,int*);
void runF(char*);
void delF(char*);
void dir();
void tweet(char*);
void copy(char*);

void main()
{
   char x[512];
   char buffer[512];
   int size;

   while(1)
   {
      interrupt(33,0,"cxxxx][===blackdos===> \0",0,0);

      /* Takes command line input */
      SCANS(x);
      PRINTS("\r\n\0");

      if(strcmp(x,"boot\0") == 1)
      {
         BOOT;
      }
      else if(strcmp(x,"cls\0") == 1)
      {
         clr();
      }
      else if(strcmp(x,"help\0") == 1)
      {
         help();
      }
      else if(strcmp(x,"echo\0") == 1)
      {
         echo(x+5);
      }
      else if(strcmp(x,"type\0") == 1)
      {
         loadF(x+5,buffer,size);
      }
      else if(strcmp(x,"run\0") == 1)
      {
         runF(x+4);
      }
      else if(strcmp(x,"del\0") == 1)
      {
         delF(x+4);
      }
      else if(strcmp(x,"dir\0") == 1)
      {
         dir();
      }
      else if(strcmp(x,"tweet\0") == 1)
      {
         tweet(x+6);
      }
      else if(strcmp(x,"cp\0") == 1)
      {
         copy(x+3);
      }
      else
      {
         interrupt(33,0,"\r\nBad command or filename\r\n\0",0,0);
      }
      interrupt(33,0,"\r\n\0",0,0);
   }
   END;
}
void copy(char* files)
{
   char fName1[7];
   char fName2[7];
   char buffer[13312];
   int size = 0;
   int i = 0;

   while(*files != ' ')
   {
      fName1[i] = *files;
      ++i;
      ++files;
   }
   fName1[i] = '\0';
   i = 0;
   ++files;

   while(*files != 0x0)
   {
      fName2[i] = *files;
      ++i;
      ++files;
   }
   fName2[i] = '\0';
   if(fName1[0] >= 'A' && fName1[0] <= 'Z')
   {
      ERROR(1);
   }
   else
   {
      LOADF(fName1,buffer,&size);
      WRITEF(fName2,buffer,size/512);
   }
}
void tweet(char* fName)
{
   char buffer[140];
   if(fName[0] >= 'A' && fName[0] <= 'Z')
   {
      ERROR(1);
   }
   else
   {
      PRINTS("Enter text to write to file, end with Enter key.\r\n\0");
      SCANS(buffer);
      WRITEF(fName,buffer,1);
   }
}

void dir()
{
   char buffer[512];
   char fname[512];
   int index = 0;
   int i;
   int runs = 0;
   int size = 0;
   int total = 0;

   READS(buffer,2);
   while(buffer[index] != 0x0)
   {
      for (i = 0; i < 6; ++i)
      {
         fname[i] = buffer[index + i];
      }
      for (i = 6; i<32; ++i)
      {
         if (buffer[index + i] == 0x0)
         {
            break;
         }
         else
         {
            ++size;
         }
      }
      total = total + size;
      /* CHeck if filename starts with a capital */
      if(fname[0] >= 'A' && fname[0] <= 'Z')
      {
      }
      else
      {
         PRINTS("File name: \0");
         PRINTS(fname);
         PRINTS("        File size: \0");
         PRINTN(size);
         PRINTS("\r\n\0");
      }
      size = 0;
      runs = runs + 6;
      index = index + 32;
   }
   PRINTS("\r\nTotal Sectors used: \0");
   PRINTN(total);
   PRINTS("\r\nSectors free: \0");
   PRINTN(48-total);
}

void delF(char* x)
{
   if(x[0] >= 'A' && x[0] <= 'Z')
   {
      ERROR(1);
   }
   else
   {
      DELF(x);
   }
}

void runF(char* x)
{
   RUNF(x,4);
}

void loadF(char* x, char* buffer, int size)
{
   buffer[0] = 0x0;
   LOADF(x,buffer,&size);
   if (buffer[0] != 0x0)
   {
      PRINTS("\r\nContents of file: \0");
      PRINTS(x);
      PRINTS("\r\n\0");
      PRINTS(buffer);
   }
}

void echo(char* x)
{
   PRINTS(x);
}

void clr()
{
   int i;

   for (i=0;i<26;++i)
   {
      interrupt(33,0,"\r\n\0",0,0);
   }
   CLEAR;
}

void help()
{
   PRINTS("\r\n\r\nHelp Manual: \r\n\r\n\0");
   PRINTS("Command:               Usage: \r\n\0");
   PRINTS("boot                   Reboot the OS.\r\n\0");
   PRINTS("cls                    Clear the screen.\r\n\0");
   PRINTS("copy <file1> <file2>   Copy file1 into file2.\r\n\0");
   PRINTS("del <file>             Delete file.\r\n\0");
   PRINTS("dir                    List disk directory.\r\n\0");
   PRINTS("echo <message>         Displays specified message.\r\n\0");
   PRINTS("help                   Displays this message.\r\n\0");
   PRINTS("run <file>             Executes specified file.\r\n\0");
   PRINTS("tweet <file>           Creates a file, and asks for text input.\r\n\0");
   PRINTS("type <file>            Displays contents of specified file.\r\n\0");
}

int strcmp(char* str1, char* str2)
{
/* Compare the char arrays index by index to see if they match */
   while(*str1 != '\0' && *str1 != ' ')
   {
      if(*str1 != *str2)
         return 0;

      *str1++;
      *str2++;
   }
   return 1;
}
