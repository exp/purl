static void xs_init (pTHX);
static PerlInterpreter *my_perl;

static int dummy_argc = 3;
static char** dummy_argv;
static char** dummy_env;

extern void GoPurl();

XS(XS_Purl)
{
  dXSARGS;
  PERL_UNUSED_VAR(cv);
  GoPurl();
  XSRETURN_EMPTY;
}

EXTERN_C void
xs_init(pTHX)
{
  char *file = __FILE__;
  newXS("Purl::Test", XS_Purl, file);
}

static void
RunPurl() {
  dummy_argv = malloc(sizeof(char*) * 3);
  dummy_env = malloc(sizeof(char*) * 2);
  dummy_argv[0] = "purl";
  dummy_argv[1] = "-e";
  dummy_argv[2] = "Purl::Test();";
  dummy_env[0] = "FOO=bar";
  dummy_env[1] = NULL;
  PERL_SYS_INIT3(&dummy_argc,&dummy_argv,&dummy_env);

  my_perl = perl_alloc();
  perl_construct(my_perl);
  PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
  perl_parse(my_perl, xs_init, dummy_argc, dummy_argv, (char **)NULL);
  perl_run(my_perl);
  perl_destruct(my_perl);
  perl_free(my_perl);
  PERL_SYS_TERM();
}