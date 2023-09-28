#define JULE_IMPL
#include <jule.h>

Jule_Value *jule_init_package(Jule_Interp *interp) {
    Jule_Value *argv_list;
    int         i;

    argv_list = jule_list_value();

    for (i = 0; i < interp->argc; i += 1) {
        argv_list->list = jule_push(argv_list->list, jule_string_value(interp, interp->argv[i]));
    }

    jule_install_var(interp, jule_get_string_id(interp, "sys:argv"), argv_list);

    return jule_string_value(interp, "sys: General system utility package.");
}
