typedef struct icvm {
	int64_t mem[4096];
	int ic;
	int flags;
#define IC_HALTED 0x1
	int64_t (*in_cb)(void *);
	void (*out_cb)(int64_t, void *);
	void *user;	/* passed to callbacks */
} Icvm;

typedef struct icarg {
	int64_t val;
	int mode;
#define IC_INDIRECT 0
#define IC_IMMEDIATE 1
} Icarg;

typedef struct icop {
	int op;
#define IC_ADD 1
#define IC_MUL 2
#define IC_IN  3
#define IC_OUT 4
#define IC_JNZ 5
#define IC_JZ  6
#define IC_LT  7
#define IC_EQ  8
#define IC_HLT 99
	Icarg args[3];
	int nin, nout;
	int nargs;
} Icop;

void ic_run(Icvm *vm, FILE *log);
void ic_step(Icvm *vm, FILE *log);

void ic_load(Icvm *vm, FILE *f);
void ic_decode(Icvm *vm, int pos, Icop *op);
int64_t *ic_arg_ptr(Icvm *vm, Icarg *arg, int pos);
void ic_exec(Icvm *vm, Icop *op);
void ic_log_pre(Icvm *vm, Icop *op, FILE *f);
void ic_log_post(Icvm *vm, Icop *op, FILE *f);
