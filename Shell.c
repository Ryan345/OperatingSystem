void main()
{
   char x[512];
   int i;
   char buffer[512];
   int size;

   while(1)
   {
      interrupt(33,0,"cxxxx][===blackdos===> \0",0,0);

      /* Takes command line input */
      interrupt(33,1,x,0,0);
      interrupt(33,0,"\r\n\0",0,0);
      if(strcmp(x,"boot\0") == 1)
      {
         interrupt(33,11,0,0,0);
      }
      else if(strcmp(x,"cls\0") == 1)
      {
         for (i=0;i<26;++i)
         {
            interrupt(33,0,"\r\n\0",0,0);
         }

         interrupt(33,12,2,8,0);
      }
      else if(strcmp(x,"help\0") == 1)
      {
         interrupt(33,0,"\r\n\r\nHelp Manual: \r\n\r\n\0",0,0);
         interrupt(33,0,"Command:               Usage: \r\n\0",0,0);
         interrupt(33,0,"boot                   Reboot the OS.\r\n\0",0,0);
         interrupt(33,0,"cls                    Clear the screen.\r\n\0",0,0);
         interrupt(33,0,"copy <file1> <file2>   Copy file1 into file2.\r\n\0",0,0);
         interrupt(33,0,"del <file>             Delete file.\r\n\0",0,0);
         interrupt(33,0,"dir                    List disk directory.\r\n\0",0,0);
         interrupt(33,0,"echo <message>         Displays specified message.\r\n\0",0,0);
         interrupt(33,0,"help                   Displays this message.\r\n\0",0,0);
         interrupt(33,0,"run <file>             Executes specified file.\r\n\0",0,0);
         interrupt(33,0,"tweet <file>           Creates a file, and asks for text input.\r\n\0",0,0);
         interrupt(33,0,"type <file>            Displays contents of specified file.\r\n\0",0,0);
      }
      else if(strcmp(x,"echo\0") == 1)
      {
         interrupt(33,0,"\r\n\0",0,0);
         interrupt(33,0,x+5,0,0);
      }
      else if(strcmp(x,"type\0") == 1)
      {
         buffer[0] = 0x0;
         interrupt(33,3,x+5,buffer,&size);
         if (buffer[0] != 0x0)
         {
            interrupt(33,0,"\r\n\r\nContents of file: \0",0,0);
            interrupt(33,0,x+5,0,0);
            interrupt(33,0,"\r\n\r\n\0",0,0);
            interrupt(33,0,buffer,0,0);
         }
      }
      else if(strcmp(x,"run\0") == 1)
      {
         interrupt(33,4,x+4,4,0);
      }
      else
      {
         interrupt(33,0,"\r\nBad command or filename\r\n\0",0,0);
      }
      interrupt(33,0,"\r\n\0",0,0);
   }
   interrupt(33,5,0,0,0);

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
