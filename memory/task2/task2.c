#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
extern char *tzname[];

int main(){
  time_t now;
  struct tm *sp;
  
  char* tz = getenv("TZ");
  int length = strlen(tz);
  char* old_tz = malloc(length + 1);
  strcpy(old_tz, tz);

  setenv("TZ", "PST8PDT", 1);
  tzset();

  (void) time(&now);
  printf("%s", ctime(&now));
  
  sp = localtime(&now);
  printf("%d/%d/%02d %d:%02d %s\n\n", 
  sp->tm_mon + 1, sp->tm_mday, 
  sp->tm_year, sp->tm_hour,
  sp->tm_min, tzname[sp->tm_isdst]);
  
  setenv("TZ", old_tz, 1);

  (void) time(&now);
  printf("%s", ctime(&now));
 
  sp = localtime(&now);
  printf("%d/%d/%02d %d:%02d %s\n", 
  sp->tm_mon + 1, sp->tm_mday, 
  sp->tm_year, sp->tm_hour,
  sp->tm_min, tzname[sp->tm_isdst]);
  
  free(old_tz);  
  return 0;
} 
   
