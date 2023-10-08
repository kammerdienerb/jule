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

static Jule_Status j_sqrt(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(sqrt(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_round(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(round(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_exp(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(exp(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_log(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(log(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_log10(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(log10(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_log2(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(log2(a->number));

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_abs(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }


    *result = jule_number_value(fabs(a->number));

    jule_free_value(a);

out:;
    return status;
}

Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("math:floor", j_floor);
    JULE_INSTALL_FN("math:ceil",  j_ceil);
    JULE_INSTALL_FN("math:pow",   j_pow);
    JULE_INSTALL_FN("math:sqrt",  j_sqrt);
    JULE_INSTALL_FN("math:round", j_round);
    JULE_INSTALL_FN("math:exp",   j_exp);
    JULE_INSTALL_FN("math:log",   j_log);
    JULE_INSTALL_FN("math:log10", j_log10);
    JULE_INSTALL_FN("math:log2",  j_log2);
    JULE_INSTALL_FN("math:abs",   j_abs);

    return jule_string_value(interp, "math: Package of math functions and constants.");
}
