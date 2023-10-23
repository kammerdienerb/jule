#define _XOPEN_SOURCE

#define JULE_IMPL
#include <jule.h>

#include <time.h>

static Jule_Status j_and(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value((long long)a->number & (long long)b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_or(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value((long long)a->number | (long long)b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_xor(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value((long long)a->number ^ (long long)b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_not(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(~(long long)a->number);

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_shl(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value((long long)a->number << (long long)b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_shr(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value((long long)a->number >> (long long)b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_hex(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    char         buff[128];

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    snprintf(buff, sizeof(buff), "%llx", (long long)a->number);
    *result = jule_string_value(interp, buff);

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status j_hexl(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;
    char         buff[128];

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    snprintf(buff, sizeof(buff), "%0*llx", (int)b->number, (long long)a->number);
    *result = jule_string_value(interp, buff);

    jule_free_value(a);

out:;
    return status;
}

Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("bit:and",  j_and);
    JULE_INSTALL_FN("bit:or",   j_or);
    JULE_INSTALL_FN("bit:xor",  j_xor);
    JULE_INSTALL_FN("bit:not",  j_not);
    JULE_INSTALL_FN("bit:shl",  j_shl);
    JULE_INSTALL_FN("bit:shr",  j_shr);
    JULE_INSTALL_FN("bit:hex",  j_hex);
    JULE_INSTALL_FN("bit:hexl", j_hexl);

    return jule_string_value(interp, "bit: bitwise operators.");
}
