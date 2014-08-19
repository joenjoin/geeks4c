/*
 * gcc -Wall -pipe -O3 -o gstack gstack.c
 *
 * A simple example to get the current stack bottom address
 * warning3 <warning3@nsfocus.com>
 * 2001-06-01
 *
 * Modified by scz <scz@nsfocus.com>
 * 2001-06-02
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

/*
 * for signal handlers
 */
typedef void Sigfunc(int);

static char * get_stack_bottom(void);
static Sigfunc * PrivateSignal(int signo, Sigfunc * func);
static void segfault(int signo);
static Sigfunc * Signal(int signo, Sigfunc * func);

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump = 0;
static Sigfunc *seg_handler;
/*
 * for xxxBSD
 */
static Sigfunc *bus_handler;

static char * get_stack_bottom(void) {
    /*
     * for autovar, must be volatile
     */
    volatile char *c;

    seg_handler = Signal( SIGSEGV, segfault);
    bus_handler = Signal( SIGBUS, segfault);
    c = (char *) &c;
    if ( sigsetjmp( jmpbuf, 1 ) != 0) {
        Signal( SIGSEGV, seg_handler);
        Signal( SIGBUS, bus_handler);
        return ((char *) c);
    }
    /*
     * now sigsetjump() is OK
     */
    canjump = 1;
    while (1) {
        *c = *c;
        c++;
    }
    return ( NULL);
} /* end of get_stack_bottom */

static Sigfunc * PrivateSignal(int signo, Sigfunc * func) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(signo, &act, &oact) < 0) {
        return ( SIG_ERR);
    }
    return (oact.sa_handler);
} /* end of PrivateSignal */

static void segfault(int signo) {
    if (0 == canjump) {
        /*
         * unexpected signal, ignore
         */
        return;
    }
    canjump = 0;
    /*
     * jump back to main, don't return
     */
    siglongjmp(jmpbuf, signo);
} /* end of segfault */

/*
 * for our signal() function
 */
static Sigfunc * Signal(int signo, Sigfunc * func) {
    Sigfunc * sigfunc;

    if ( SIG_ERR == (sigfunc = PrivateSignal(signo, func))) {
        exit( EXIT_FAILURE);
    }
    return (sigfunc);
} /* end of Signal */

int main(int argc, char * argv[]) {
    fprintf(
    stderr, "Current stack bottom is %p\n", get_stack_bottom());

    while (1)
        ;
    return ( EXIT_SUCCESS);
} /* end of main */
