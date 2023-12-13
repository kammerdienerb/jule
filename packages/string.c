#define _XOPEN_SOURCE

#define JULE_IMPL
#include <jule.h>

#include <time.h>

static Jule_Status j_split(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int         status;
    Jule_Value *s;
    Jule_Value *t;
    char       *cpy;
    const char *delim;
    const char *tok;

    status = jule_args(interp, tree, "ss", n_values, values, &s, &t);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    cpy   = jule_charptr_dup(jule_get_string(interp, s->string_id)->chars);
    delim = jule_get_string(interp, t->string_id)->chars;

    *result = jule_list_value();

    for (tok = strtok(cpy, delim); tok != NULL; tok = strtok(NULL, delim)) {
        (*result)->list = jule_push((*result)->list, jule_string_value(interp, tok));
    }

    JULE_FREE(cpy);

    jule_free_value(s);
    jule_free_value(t);

out:;
    return status;
}

static Jule_Status j_splits(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int                 status;
    Jule_Value         *s;
    Jule_Value         *t;
    char               *cpy;
    const char         *delim;
    unsigned long long  delim_len;
    const char         *tok;
    char               *next;
    char                c;

    status = jule_args(interp, tree, "ss", n_values, values, &s, &t);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    cpy       = jule_charptr_dup(jule_get_string(interp, s->string_id)->chars);
    delim     = jule_get_string(interp, t->string_id)->chars;
    delim_len = jule_get_string(interp, t->string_id)->len;

    *result = jule_list_value();

    tok = cpy;

    while ((next = strstr(tok, delim)) != NULL) {
        c = *next;
        *next = 0;
        (*result)->list = jule_push((*result)->list, jule_string_value(interp, tok));
        *next = c;
        tok = next + delim_len;
    }
    (*result)->list = jule_push((*result)->list, jule_string_value(interp, tok));

    JULE_FREE(cpy);

    jule_free_value(s);
    jule_free_value(t);

out:;
    return status;
}

static Jule_Status j_replace(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int         status;
    Jule_Value *s;
    Jule_Value *a;
    Jule_Value *b;
    const char *base;
    const char *old;
    const char *new;
    char       *str;
    const char *found;

    status = jule_args(interp, tree, "sss", n_values, values, &s, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    base = jule_get_string(interp, s->string_id)->chars;
    old  = jule_get_string(interp, a->string_id)->chars;
    new  = jule_get_string(interp, b->string_id)->chars;

    str = JULE_MALLOC(strlen(base) + 1);

    str[0] = 0;

    if (strlen(old) > 0) {
        while ((found = strstr(base, old)) != NULL) {
            str = JULE_REALLOC(str, strlen(str) + (found - base) + 1);
            strncat(str, base, (found - base));
            str = JULE_REALLOC(str, strlen(str) + strlen(new) + 1);
            strcat(str, new);
            base = found + strlen(old);
        }
    }

    str = JULE_REALLOC(str, strlen(str) + strlen(base) + 1);
    strcat(str, base);

    *result = jule_string_value(interp, str);

    JULE_FREE(str);

    jule_free_value(s);
    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status j_trim(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int                 status;
    Jule_Value         *s;
    char               *cpy;
    char               *p;
    unsigned long long  len;

    status = jule_args(interp, tree, "s", n_values, values, &s);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    cpy = jule_charptr_dup(jule_get_string(interp, s->string_id)->chars);
    p   = cpy;

    while (*p && jule_is_space(*p)) { p += 1; }
    len = strlen(p);
    while (len && jule_is_space(p[len - 1])) {
        p[len - 1] = 0;
        len -= 1;
    }

    *result = jule_string_value(interp, p);

    JULE_FREE(cpy);

    jule_free_value(s);

out:;
    return status;
}

static Jule_Status j_index(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int                 status;
    Jule_Value         *haystack;
    Jule_Value         *needle;
    const char         *h;
    const char         *n;
    const char         *s;

    status = jule_args(interp, tree, "ss", n_values, values, &haystack, &needle);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    h = jule_get_string(interp, haystack->string_id)->chars;
    n = jule_get_string(interp, needle->string_id)->chars;

    s = strstr(h, n);

    if (s == NULL) {
        *result = jule_nil_value();
    } else {
        *result = jule_number_value(s - h);
    }

    jule_free_value(haystack);
    jule_free_value(needle);

out:;
    return status;
}

static Jule_Status j_contains(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    int                 status;
    Jule_Value         *haystack;
    Jule_Value         *needle;
    const char         *h;
    const char         *n;
    const char         *s;

    status = jule_args(interp, tree, "ss", n_values, values, &haystack, &needle);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    h = jule_get_string(interp, haystack->string_id)->chars;
    n = jule_get_string(interp, needle->string_id)->chars;

    s = strstr(h, n);

    *result = jule_number_value(s != NULL);

    jule_free_value(haystack);
    jule_free_value(needle);

out:;
    return status;
}

Jule_Value *jule_init_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("string:split",    j_split);
    JULE_INSTALL_FN("string:splits",   j_splits);
    JULE_INSTALL_FN("string:replace",  j_replace);
    JULE_INSTALL_FN("string:trim",     j_trim);
    JULE_INSTALL_FN("string:index",    j_index);
    JULE_INSTALL_FN("string:contains", j_contains);

    return jule_string_value(interp, "string: String wrangling package.");
}
