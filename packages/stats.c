#define JULE_IMPL
#include <jule.h>

#include <math.h>

static Jule_Status calc_mean(Jule_Interp *interp, Jule_Value *list, double *out) {
    Jule_Status  status;
    double       mean;
    Jule_Value  *it;

    status = JULE_SUCCESS;

    mean = 0.0;

    FOR_EACH(list->list, it) {
        if (it->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, it, JULE_NUMBER, it->type);
            goto out;
        }
        mean += it->number;
    }

    mean /= jule_len(list->list);

    *out = mean;

out:;
    return status;
}

static Jule_Status calc_var(Jule_Interp *interp, Jule_Value *list, double *out) {
    Jule_Status  status;
    double       mean;
    double       var;
    Jule_Value  *it;
    double       d;

    status = calc_mean(interp, list, &mean);
    if (status != JULE_SUCCESS) {
        goto out;
    }

    var = 0.0;

    FOR_EACH(list->list, it) {
        d    = it->number - mean;
        var += d * d;
    }

    var /= jule_len(list->list);

    *out = var;

out:;
    return status;
}

static Jule_Status j_mean(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    double       mean;

    status = jule_args(interp, tree, "l", n_values, values, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    status = calc_mean(interp, list, &mean);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free;
    }

    *result = jule_number_value(mean);

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status j_var(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    double       var;

    status = jule_args(interp, tree, "l", n_values, values, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    status = calc_var(interp, list, &var);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free;
    }

    *result = jule_number_value(var);

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status j_std(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    double       var;

    status = jule_args(interp, tree, "l", n_values, values, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    status = calc_var(interp, list, &var);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free;
    }

    *result = jule_number_value(sqrt(var));

out_free:;
    jule_free_value(list);

out:;
    return status;
}


Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("stats:mean", j_mean);
    JULE_INSTALL_FN("stats:var",  j_var);
    JULE_INSTALL_FN("stats:std",  j_std);

    return jule_string_value(interp, "stats: Statistical analysis functions.");
}
