# Developments in C
Samples in C computer language

## Directories

### Includes
In these directories, you can find simple C include files, usable for more beautiful and readable codes.

They can be used directly with the C pre-compiler.

* [incboolean](incboolean): Definition of Boolean type
* [incmpfr](incmpfr): Use [MPFR library](https://www.mpfr.org/) with macros

### Libraries
In these directories, you found C compilable codes.

Some codes were written during my studies.

* [libmybacktrace](libmybacktrace): Simple example of backtrace & backtrace_symbols functions use case (as-is in man page)
* [libmylog](libmylog): Minimalist log library
* [libmyprocess](libmyprocess): fork and daemonize (setsid, fork, dup, umask & chdir) functions and test program
* [libmysrv](libmysrv): Functions for building a basic TCP server with socket, bind, accept, fork, waitpid... functions
* [libmythread](libmythread): Work with threads (getid/SYS_gettid with syscall) and simple examples (create, join, mutex...)
* [libproc](libproc): Access to "`/proc`" directory (static and dynamic compilation and link examples)

### Examples

* [dl](dl): Test of dynamic library load and dynamic symbol load with dlopen, dlsym and dlclose functions
* [memory](memory): Simple for printing size of types (compilation possible in 32 bits and in 64 bits modes)
* [signal](signal): Test handlers for SIGINT, SIGQUIT and SIGALARM signals
* [va_args](va_args): Test of variable argument lists (special '...' function parameter) with va_start, va_arg and va_end functions/macros

### Projects
* [myhome](myhome): Library and test program for read and write on a [My Home](http://www.homesystems-legrandgroup.com/) (Legrand, BTicino...) bus with [Open Web Net](https://www.myopen-legrandgroup.com/) (OWN) language.

### Make files
* [make](make): Set of make files essentially for compiling C sources
* * Note: Auto-dependency generation is based on the [Paul D. Smith's work](http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/). Thanks!
