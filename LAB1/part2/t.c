#include <stdio.h>

typedef unsigned int u32;
  char *ctable = "0123456789ABCDEF";
  int BASE = 10;
  
  int rpu(u32 x, u32 base)//generates the digits of x%10 in ASCII recursively
			  //and prints them on the return path (helper funtion)
  {
    char c;
    if(x){
      c = ctable[x % BASE];
      rpu(x / BASE, BASE);
      putchar(c);
    }
  }
  int printu(u32 x)
  {
    if(x==0)
      putchar('0');
    else
      rpu(x, 10);
  }
  int printd(int x)
  {
    if(x==0)
      putchar('0');
    else if (x < 0)
    {
      putchar('-');
      rpu(-x,10);
    }
    else
      rpu(x, 10);
  }
  int  printx(u32 x)
  {
    putchar('0');
    putchar('x');
    if(x==0)
      putchar('0');
    else
      rpu(x, 16);
  }
  int  printo(u32 x)
  {
    putchar('0');
    if(x==0)
      putchar('0');
    else
      rpu(x, 8);
  }
  int prints(char *s)
  {
    char *cp = s;
    while(*cp)
      putchar(*cp++);
  }
  int myprintf(char *fmt, ...)
  { //                12    8      4      0
    // high... d | c | b | a | fmt | retPC | ebp | locals ...low
    int *ebp = (int*)getebp();//stack frame pointer
    char *cp = fmt;//pointer to fmt points at the format string

    //each int ptr increment = 4 bytes
    //12/4=3
    int *ip = (int *)&fmt+1;//pointer to first item on stack

    while(*cp)
    {
      if (*cp != '%')//using cp to scan the format string
      {
        //for each \n, spit out an extra \r
        if (putchar(*cp)=='\n')
          putchar('\r');
          
      }
      else // *cp == %
      {
        cp++; // bypass %
	switch(*cp) // *cp = char after %
        {
	  case 'c': //char
            putchar(*ip);
	    ip++;
            break;
	  
	  case 's': //string
            prints((char *)(*ip));
	    ip++;
            break;

	  case 'd': //int
            printd(*ip);
	    ip++;
            break;

	  case 'o': //oct
            printo(*ip);
	    ip++;
            break;

	  case 'x': //hex
            printx(*ip);
	    ip++;
            break;

	  case 'u': //unsigend
            printu(*ip);
	    ip++;
            break;

	  case '%':
            putchar('%');
            break;

	  default: 
	    putchar('%');
	    putchar(*cp);
	    break;
        }//switch
      }//else
	cp++;
    }//while
  }//myprintf 



int main(int argc, char *argv[ ], char *env[ ])
{
  
  
  int i;
  myprintf("argc=%d\n", argc);
  //print argv
  for (i=0; i < argc; i++)
    myprintf("argv[%d] = %s\n", i, argv[i]);
  myprintf("...");
  getchar();

  //print env[]
  myprintf("---- env strings ----\n");
  i = 0;
  while(env[i])
  {
    myprintf("env[%d] = %s\n", i, env[i]);
    i++;
  }
  
  myprintf("cha=%c string=%s      dec=%d hex=%x oct=%o neg=%d\n", 
	       'A', "this is a test", 100,    100,   100,  -100);

}

