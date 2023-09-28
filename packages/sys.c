#define JULE_IMPL
#include <jule.h>

static int    sys_argc;
static char **sys_argv;

__attribute__((constructor)) void get_args(int argc, char **argv) {
    sys_argc = argc;
    sys_argv = argv;
}


Jule_Value *jule_load_package(Jule_Interp *interp) {
    Jule_Value *argv_list;
    int         i;

    argv_list = jule_list_value();

    for (i = 0; i < sys_argc; i += 1) {
        argv_list->list = jule_push(argv_list->list, jule_string_value(interp, sys_argv[i]));
    }

    jule_install_var(interp, jule_get_string_id(interp, "sys:argv"), argv_list);

    return jule_string_value(interp, "sys: General system utility package.");
}
