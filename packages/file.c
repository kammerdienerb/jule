#define JULE_IMPL
#include <jule.h>

#include <stdio.h>

static Jule_Status _j_open(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result, const char *mode) {
    Jule_Status        status;
    Jule_Value        *pathv;
    const Jule_String *path;
    FILE              *f;
    Jule_Value        *handle;

    status = jule_args(interp, tree, "s", n_values, values, &pathv);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    path = jule_get_string(interp, pathv->string_id);

    f = fopen(path->chars, mode);

    if (f == NULL) {
        *result = jule_nil_value();
        goto out_free;
    }

    handle = jule_number_value(0);
    handle->_integer = (unsigned long long)(void*)f;

    *result = jule_object_value();
    jule_insert(*result, jule_string_value(interp, "__handle__"), handle);
    jule_insert(*result, jule_string_value(interp, "path"),       jule_copy_force(pathv));

out_free:;
    jule_free_value(pathv);

out:;
    return status;
}

static Jule_Status j_open_rd(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    return _j_open(interp, tree, n_values, values, result, "r");
}

static Jule_Status j_open_wr(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    return _j_open(interp, tree, n_values, values, result, "w");
}

static Jule_Status j_close(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *file;
    Jule_Value  *key;
    Jule_Value  *handle;
    FILE        *f;

    status = jule_args(interp, tree, "o", n_values, values, &file);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    key    = jule_string_value(interp, "__handle__");
    handle = jule_field(file, key);
    jule_free_value(key);

    f = handle == NULL ? NULL : (void*)handle->_integer;

    jule_free_value(file);

    if (f == NULL) {
        *result = jule_nil_value();
        goto out;
    }

    fclose(f);
    *result             = jule_number_value(0);
    (*result)->_integer = (unsigned long long)(void*)f;

out:;
    return status;
}

static Jule_Status j_read_line(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *file;
    Jule_Value  *key;
    Jule_Value  *handle;
    FILE        *f;
    char        *line;
    size_t       cap;
    size_t       len;

    status = jule_args(interp, tree, "o", n_values, values, &file);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    key    = jule_string_value(interp, "__handle__");
    handle = jule_field(file, key);
    jule_free_value(key);

    f = handle == NULL ? NULL : (void*)handle->_integer;

    jule_free_value(file);

    if (f == NULL) {
        *result = jule_nil_value();
        goto out;
    }

    line = NULL;
    cap  = 0;

    if (getline(&line, &cap, f) < 0) {
        *result = jule_nil_value();
        goto out;
    }

    len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = 0;
    }

    *result = jule_string_value(interp, line);

    free(line);

out:;
    return status;
}

static Jule_Status j_read_lines(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *file;
    Jule_Value  *key;
    Jule_Value  *handle;
    FILE        *f;
    char        *line;
    size_t       cap;
    size_t       len;

    status = jule_args(interp, tree, "o", n_values, values, &file);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    key    = jule_string_value(interp, "__handle__");
    handle = jule_field(file, key);
    jule_free_value(key);

    f = handle == NULL ? NULL : (void*)handle->_integer;

    jule_free_value(file);

    if (f == NULL) {
        *result = jule_nil_value();
        goto out;
    }

    *result = jule_list_value();

    line = NULL;
    cap  = 0;
    while (getline(&line, &cap, f) > 0) {
        len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = 0;
        }

        (*result)->list = jule_push((*result)->list, jule_string_value(interp, line));
    }

    if (line != NULL) {
        free(line);
    }

out:;
    return status;
}

static Jule_Status j_write(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *file;
    Jule_Value        *string;
    Jule_Value        *key;
    Jule_Value        *handle;
    const Jule_String *s;
    FILE              *f;
    size_t             r;

    status = jule_args(interp, tree, "os", n_values, values, &file, &string);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    key    = jule_string_value(interp, "__handle__");
    handle = jule_field(file, key);
    s      = jule_get_string(interp, string->string_id);
    jule_free_value(key);
    jule_free_value(string);

    f = handle == NULL ? NULL : (void*)handle->_integer;

    jule_free_value(file);

    if (f == NULL) {
        *result = jule_nil_value();
        goto out;
    }

    r = fwrite(s->chars, 1, s->len, f);
    fflush(f);

    *result = jule_number_value(r);

out:;
    return status;
}

Jule_Value *jule_load_package(Jule_Interp *interp) {
#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("file:open",       j_open_rd);
    JULE_INSTALL_FN("file:open-rd",    j_open_rd);
    JULE_INSTALL_FN("file:open-wr",    j_open_wr);
    JULE_INSTALL_FN("file:close",      j_close);
    JULE_INSTALL_FN("file:read-line",  j_read_line);
    JULE_INSTALL_FN("file:read-lines", j_read_lines);
    JULE_INSTALL_FN("file:write",      j_write);

    return jule_string_value(interp, "file: File IO package.");
}
