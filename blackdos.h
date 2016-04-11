#define PRINTS(x)     interrupt(33,0,x,0,0)
#define PRINTN(x)     interrupt(33,13,x,0,0)
#define SCANS(x)      interrupt(33,1,x,0,0)
#define SCANN(x)      interrupt(33,14,&x,0,0)
#define LOADF(x,y,z)  interrupt(33,3,x,y,z)
#define RUNF(x,y)     interrupt(33,4,x,y,0)
#define END           interrupt(33,5,0,0,0)
#define CLEAR         interrupt(33,12,2,8,0)
#define BOOT          interrupt(33,11,0,0,0)
#define DELF(x)       interrupt(33,7,x,0,0)
#define READS(x,y)    interrupt(33,2,x,y,0)
#define WRITEF(x,y,z) interrupt(33,8,x,y,z)
#define ERROR(x)      interrupt(33,15,x,0,0)
