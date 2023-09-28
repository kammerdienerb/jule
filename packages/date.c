#define JULE_IMPL
#include <jule.h>

#include <time.h>

static Jule_Status j_parse_iso(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int         status;
    Jule_Value *s;
    struct tm   tm;
    const char *weekdays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    status = jule_args(interp, tree, "s", n_values, values, &s);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    memset(&tm, 0, sizeof(tm));
    if (strptime(jule_get_string(interp, s->string_id)->chars, "%FT%T%z", &tm) == NULL) {
        *result = jule_nil_value();
        goto out_free;
    }

    *result = jule_object_value();

    jule_insert(*result, jule_string_value(interp, "seconds"), jule_number_value(tm.tm_sec));
    jule_insert(*result, jule_string_value(interp, "minutes"), jule_number_value(tm.tm_min));
    jule_insert(*result, jule_string_value(interp, "hours"),   jule_number_value(tm.tm_hour));
    jule_insert(*result, jule_string_value(interp, "day"),     jule_number_value(tm.tm_mday));
    jule_insert(*result, jule_string_value(interp, "wday"),    jule_string_value(interp, weekdays[tm.tm_wday % 7]));
    jule_insert(*result, jule_string_value(interp, "yday"),    jule_number_value(1 + tm.tm_yday));
    jule_insert(*result, jule_string_value(interp, "month"),   jule_number_value(1 + tm.tm_mon));
    jule_insert(*result, jule_string_value(interp, "year"),    jule_number_value(1900 + tm.tm_year));
    jule_insert(*result, jule_string_value(interp, "dst"),     jule_number_value(tm.tm_isdst));
    jule_insert(*result, jule_string_value(interp, "epoch"),   jule_number_value(mktime(&tm)));

out_free:;
    jule_free_value(s);

out:;
    return status;
}

Jule_Value *jule_load_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("date:parse-iso", j_parse_iso);

    return jule_string_value(interp, "date: Date and time package.");
}
