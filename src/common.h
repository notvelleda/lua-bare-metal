/* === definitions forcefully shared across all source files === */

/* hardcoded signal type since libc doesn't have whatever this defaults to */
#define l_signalT int

/* hard-coded decimal point since libc doesn't have locale support */
#define lua_getlocaledecpoint() ('.')

/* forces lua to use longjmp and setjmp builtins since libc doesn't provide them */
#define LUAI_THROW(L,c) __builtin_longjmp((c)->b, 1)
#define LUAI_TRY(L,c,a) if (__builtin_setjmp((c)->b) == 0) { a }
#define luai_jmpbuf jmp_buf

void write_string(const char *string, int len);

#define lua_writestring(s,l) write_string(s, l)
#define lua_writeline() write_string("\n", 1)

/* stubs to allow lauxlib.c to compile */
#define BUFSIZ 1
#define stdin NULL
#define stderr NULL

#define fprintf(f,...) printf(__VA_ARGS__)
