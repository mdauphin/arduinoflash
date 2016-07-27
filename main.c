#include "avrdude.h"
#include "libavrdude.h"
#include "stk500.h"
#include "arduino.h"

int    verbose;     /* verbose output */
char * progname = NULL;
char   progbuf[PATH_MAX]; /* temporary buffer of spaces the same
                             length as progname; used for lining up
                             multiline messages */


int avrdude_message(const int msglvl, const char *format, ...)
{
    int rc = 0;
    va_list ap;
    if (verbose >= msglvl) {
        va_start(ap, format);
        rc = vfprintf(stderr, format, ap);
        va_end(ap);
    }
    return rc;
}

int usleep(unsigned int usec)
{ 
	Sleep(usec * 1000);
	return 0;
}

#define uint64_t unsigned long long
int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}


int main(int argc, char** argv)
{
	size_t              len;         /* length for various strings */
	int              i;           /* general loop counter */
	int              rc;          /* general return code checking */
	int     init_ok;     /* Device initialization worked well */

	PROGRAMMER *pgm;
	struct avrpart * p;           /* which avr part we are programming */
	char  * partdesc;    /* part id */


	char atmega168_part[] = "atmega168";
	partdesc = atmega168_part;

	if (!progname) progname = strrchr(argv[0],'\\');

  len = strlen(progname) + 2;
  for (i=0; i<len; i++)
    progbuf[i] = ' ';
  progbuf[i] = 0;	

  pgm = pgm_new();

  arduino_initpgm(pgm);

  if (pgm->setup) {
	  pgm->setup(pgm);
  }
	
  p = locate_part(part_list, partdesc);

  pgm->baudrate = 115200;
  //pgm->bitclock = 0;
  //pgm->ispdelay = ispdelay;

  rc = pgm->open(pgm, "COM3");


  pgm->enable(pgm);

  pgm->rdy_led(pgm, OFF);
  pgm->err_led(pgm, OFF);
  pgm->pgm_led(pgm, OFF);
  pgm->vfy_led(pgm, OFF);


  init_ok = (rc = pgm->initialize(pgm, p)) >= 0;


  pgm->rdy_led(pgm, ON);

  //sleep

  rc = avr_signature(pgm, p);

}