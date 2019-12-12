#ifdef _WIN32
# define NO_ERR
# define NO_GETOPT
#endif

#ifdef NO_ERR
__declspec(noreturn) void err(int status, char *fmt, ...);
__declspec(noreturn) void errx(int status, char *fmt, ...);
#endif

#ifdef NO_GETOPT
int getopt(int argc, char **argv, const char *optstring);

extern char *optarg;
extern int opterr;
extern int optind;
extern int optopt;
extern int optreset;
#endif

#ifdef _WIN32
int _isatty(int fd);
# define isatty _isatty
#endif
