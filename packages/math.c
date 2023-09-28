#define JULE_IMPL
#include <jule.h>

#include <math.h>

static Jule_Status j_floor(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(floor(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_ceil(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(ceil(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_pow(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(pow(a->number, b->number));

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("math:floor", j_floor);
    JULE_INSTALL_FN("math:ceil",  j_ceil);
    JULE_INSTALL_FN("math:pow",   j_pow);

    return jule_string_value(interp, "math: Package of math functions and constants.");
}
