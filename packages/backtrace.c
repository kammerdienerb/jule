#define _XOPEN_SOURCE

#define JULE_IMPL
#include <jule.h>

#include <time.h>

static Jule_Status j_backtrace_get(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int                   status;
    int                   i;
    Jule_Backtrace_Entry *it;
    char                 *s;
    char                  buff[4096];

    status = JULE_SUCCESS;

    (void)values;
    if (n_values != 0) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 0, n_values, 0);
        *result = NULL;
        goto out;
    }

    *result = jule_list_value();

    for (i = jule_len(interp->backtrace); i > 0; i -= 1) {
        it = jule_elem(interp->backtrace, i - 1);

        s = jule_to_string(interp, it->fn, 0);
        snprintf(buff, sizeof(buff), "%s:%u:%u %s",
                 it->file == NULL ? "<?>" : it->file->chars,
                 it->fn->line,
                 it->fn->col,
                 s);
        JULE_FREE(s);
        (*result)->list = jule_push((*result)->list, jule_string_value(interp, buff));
    }

out:;
    return status;
}

Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("backtrace:get", j_backtrace_get);

    return jule_string_value(interp, "backtrace: Get the current backtrace.");
}
