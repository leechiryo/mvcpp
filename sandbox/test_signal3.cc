/* Example of using sigaction() to setup a signal handler with 3 arguments
 * including siginfo_t.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <thread>
#include <memory>
#include <setjmp.h>
 
using namespace std;

thread_local sigjmp_buf jbuf;

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
  siglongjmp(jbuf, sig);
}

class Some
{
  int a=0;
  public:
  void seta()
  {
    a=1;
  }
};

/* a buggy function in another thread. */
void bugfunc()
{
  int sysfault = 0;
  if((sysfault = sigsetjmp(jbuf, 1)) == 0)
  {
    for(int i=0; i<3; i++)
    {
      printf("I'm a buggy thread. Will have trouble soon or after.\n");
      sleep(1);
    }
    Some * so = (Some*)112233;
    so->seta(); /* segment violation here. */
  }
  else
  {
    /* error process here */
    printf("%d error is occured, so the process is skipped.\n", sysfault);
  }
}

void bugfunc2()
{
  int sysfault = 0;
  if((sysfault = sigsetjmp(jbuf, 1)) == 0)
  {
    for(int i=0; i<3; i++)
    {
      printf("I'm a buggy thread too. Will have trouble soon or after.\n");
      sleep(1);
    }
    int dvd = 100;
    int dvi = 0;
    int res = dvd / dvi;/* devide by 0 here. */
  }
  else
  {
    /* error process here */
    printf("%d error is occured, so the process is skipped.\n", sysfault);
  }
}

void goodfunc()
{
  if(sigsetjmp(jbuf, 1) == 0)
  {
    while(true)
    {
      sleep(1);
      printf("Hi, I'm a good thread.\n");
    }
  }
  else
  {
    printf("If I'm not good, kill me here.");
  }
}
 
int main (int argc, char *argv[])
{
  struct sigaction act;

  memset (&act, '\0', sizeof(act));

  /* Use the sa_sigaction field because the handles has two additional parameters */
  act.sa_sigaction = &hdl;

  /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
  act.sa_flags = SA_SIGINFO;

  if (sigaction(SIGBUS, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGFPE, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGILL, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGSEGV, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }

  thread worker2 = thread(goodfunc);
  worker2.detach();

  thread worker = thread(bugfunc);
  worker.detach();

  for(int i=0; i<5; i++)
  {
    thread worker3 = thread(bugfunc);
    worker3.detach();
  }

  for(int i=0; i<5; i++)
  {
    thread worker3 = thread(bugfunc2);
    worker3.detach();
  }

  while(true)
  {
    printf("main thread, won't be terminated so easily.\n");
    sleep(1);
  }

  return 0;
}
