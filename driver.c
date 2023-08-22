#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define JULE_IMPL
#include "jule.h"

Jule_Interp interp;

static int map_file_into_readonly_memory(const char *path, const char **addr, int *size);
static void on_jule_error(Jule_Error_Info *info);

int main(int argc, char **argv) {
    const char *code;
    int         code_size;

    if (argc != 2) {
        fprintf(stderr, "expected one argument: a jule file path\n");
        return 1;
    }

    if (map_file_into_readonly_memory(argv[1], &code, &code_size)) {
        fprintf(stderr, "error opening '%s'\n", argv[1]);
        return 1;
    }

    jule_init_interp(&interp);
    jule_set_error_callback(&interp, on_jule_error);
    jule_parse(&interp, code, strlen(code));
    jule_interp(&interp);
    jule_free(&interp);

    return 0;
}


static int map_file_into_readonly_memory(const char *path, const char **addr, int *size) {
    int          status;
    FILE        *f;
    int          fd;
    struct stat  fs;

    status = 0;
    f      = fopen(path, "r");

    if (f == NULL) { status = 1; goto out; }

    fd = fileno(f);

    if      (fstat(fd, &fs) != 0) { status = 2; goto out_fclose; }
    else if (S_ISDIR(fs.st_mode)) { status = 3; goto out_fclose; }

    *size = fs.st_size;

    if (*size == 0) {
        *addr = NULL;
        goto out_fclose;
    }

    *addr = mmap(NULL, *size, PROT_READ, MAP_SHARED, fd, 0);

    if (*addr == MAP_FAILED) { return 4; }

out_fclose:
    fclose(f);

out:
    return status;
}

static void on_jule_error(Jule_Error_Info *info) {
    Jule_Status  status;
    char        *s;

    status = info->status;

    fprintf(stderr, "Jule Error: %s\n", jule_error_string(status));
    switch (status) {
        case JULE_ERR_UNEXPECTED_EOS:
        case JULE_ERR_UNEXPECTED_TOK:
            fprintf(stderr, "    LINE:   %d, COLUMN: %d\n", info->location.line, info->location.col);
            break;
        default:
            fprintf(stderr, "    LINE:   %d\n", info->location.line);
            break;
    }
    switch (status) {
        case JULE_ERR_LOOKUP:
            fprintf(stderr, "    SYMBOL: %s\n", info->sym);
            break;
        case JULE_ERR_ARITY:
            fprintf(stderr, "    WANTED: %s%d\n", info->arity_at_least ? "at least " : "", info->wanted_arity);
            fprintf(stderr, "    GOT:    %d\n", info->got_arity);
            break;
        case JULE_ERR_TYPE:
            fprintf(stderr, "    WANTED: %s\n", jule_type_string(info->wanted_type));
            fprintf(stderr, "    GOT:    %s\n", jule_type_string(info->got_type));
            break;
        case JULE_ERR_OBJECT_KEY_TYPE:
            fprintf(stderr, "    WANTED: number or string\n");
            fprintf(stderr, "    GOT:    %s\n", jule_type_string(info->got_type));
            break;
        case JULE_ERR_NOT_A_FN:
            fprintf(stderr, "    GOT:    %s\n", jule_type_string(info->got_type));
            break;
        case JULE_ERR_BAD_INDEX:
            s = jule_to_string(info->bad_index, 0);
            fprintf(stderr, "    INDEX:  %s\n", s);
            JULE_FREE(s);
            break;
        default:
            break;
    }

    jule_free_error_info(info);
    exit(status);
}
