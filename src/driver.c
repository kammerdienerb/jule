#include "whereami.c"

#define _XOPEN_SOURCE 700
#include <stdio.h>

#define JULE_IMPL
#include "jule.h"

#include <limits.h>
#include <stdlib.h>
#include <libgen.h>

Jule_Interp interp;

static void on_jule_error(Jule_Error_Info *info);

int main(int argc, char **argv) {
    const char *code;
    int         code_size;
    int         exe_path_length;
    char       *exe_path;

    if (argc < 2) {
        fprintf(stderr, "expected at least one argument: a jule file path\n");
        return 1;
    }

    if (jule_map_file_into_readonly_memory(argv[1], &code, &code_size)) {
        fprintf(stderr, "error opening '%s'\n", argv[1]);
        return 1;
    }


    jule_init_interp(&interp);
    jule_set_error_callback(&interp, on_jule_error);
    jule_set_argv(&interp, argc - 1, argv + 1);
    interp.cur_file = jule_get_string_id(&interp, argv[1]);


    exe_path_length = wai_getExecutablePath(NULL, 0, NULL);
    if (exe_path_length >= 0) {
        exe_path        = malloc(exe_path_length + 1 + strlen("/packages"));
        wai_getExecutablePath(exe_path, exe_path_length, NULL);
        exe_path[exe_path_length] = 0;

        dirname(exe_path);
        strcat(exe_path, "/packages");

        jule_add_package_directory(&interp, exe_path);
    }

    jule_parse(&interp, code, strlen(code));
    jule_interp(&interp);
    jule_free(&interp);

    return 0;
}


static void on_jule_error(Jule_Error_Info *info) {
    Jule_Status  status;
    char        *s;

    status = info->status;

    fprintf(stderr, "%s:%u:%u: error: %s",
            info->file == NULL ? "<?>" : info->file,
            info->location.line,
            info->location.col,
            jule_error_string(status));

    switch (status) {
        case JULE_ERR_LOOKUP:
        case JULE_ERR_RELEASE_WHILE_BORROWED:
            fprintf(stderr, " (%s)", info->sym);
            break;
        case JULE_ERR_ARITY:
            fprintf(stderr, " (wanted %s%d, got %d)",
                    info->arity_at_least ? "at least " : "",
                    info->wanted_arity,
                    info->got_arity);
            break;
        case JULE_ERR_TYPE:
            fprintf(stderr, " (wanted %s, got %s)",
                    jule_type_string(info->wanted_type),
                    jule_type_string(info->got_type));
            break;
        case JULE_ERR_OBJECT_KEY_TYPE:
            fprintf(stderr, " (wanted number or string, got %s)", jule_type_string(info->got_type));
            break;
        case JULE_ERR_BAD_INVOKE:
            fprintf(stderr, " (got %s)", jule_type_string(info->got_type));
            break;
        case JULE_ERR_BAD_INDEX:
            s = jule_to_string(info->interp, info->bad_index, 0);
            fprintf(stderr, " (index: %s)", s);
            JULE_FREE(s);
            break;
        case JULE_ERR_FILE_NOT_FOUND:
        case JULE_ERR_FILE_IS_DIR:
        case JULE_ERR_MMAP_FAILED:
            fprintf(stderr, " (%s)", info->path);
            break;
        case JULE_ERR_LOAD_PACKAGE_FAILURE:
            fprintf(stderr, " (%s) %s", info->path, info->package_error_message);
            break;
        default:
            break;
    }

    fprintf(stderr, "\n");

    jule_free_error_info(info);
    exit(status);
}
