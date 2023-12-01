#ifndef __JULE_H__
#define __JULE_H__

#define _JULE_STATUS                                                                                                            \
    _JULE_STATUS_X(JULE_SUCCESS,                             "No error.")                                                       \
    _JULE_STATUS_X(JULE_ERR_UNEXPECTED_EOS,                  "Unexpected end of input.")                                        \
    _JULE_STATUS_X(JULE_ERR_UNEXPECTED_TOK,                  "Unexpected token.")                                               \
    _JULE_STATUS_X(JULE_ERR_LINE_TOO_LONG,                   "Token starts too far into a line (column too big).")              \
    _JULE_STATUS_X(JULE_ERR_TOO_MANY_LINES,                  "You have exceeded the maximum number of lines in a single file.") \
    _JULE_STATUS_X(JULE_ERR_EXTRA_RPAREN,                    "Extraneous closing parenthesis.")                                 \
    _JULE_STATUS_X(JULE_ERR_MISSING_RPAREN,                  "End of line while parentheses left open.")                        \
    _JULE_STATUS_X(JULE_ERR_EMPTY_PARENS,                    "Empty parentheses are not allowed.")                              \
    _JULE_STATUS_X(JULE_ERR_NO_INPUT,                        "Missing a top-level expression.")                                 \
    _JULE_STATUS_X(JULE_ERR_LOOKUP,                          "Failed to find symbol.")                                          \
    _JULE_STATUS_X(JULE_ERR_BAD_INVOKE,                      "Invoked value is not something that can be invoked in this way.") \
    _JULE_STATUS_X(JULE_ERR_ARITY,                           "Incorrect number of arguments.")                                  \
    _JULE_STATUS_X(JULE_ERR_TYPE,                            "Incorrect argument type.")                                        \
    _JULE_STATUS_X(JULE_ERR_OBJECT_KEY_TYPE,                 "Expression is not a valid key type.")                             \
    _JULE_STATUS_X(JULE_ERR_MISSING_VAL,                     "Missing value expression.")                                       \
    _JULE_STATUS_X(JULE_ERR_BAD_INDEX,                       "Field or element not found.")                                     \
    _JULE_STATUS_X(JULE_ERR_EVAL_CANCELLED,                  "Evaluation was cancelled.")                                       \
    _JULE_STATUS_X(JULE_ERR_FILE_NOT_FOUND,                  "File not found.")                                                 \
    _JULE_STATUS_X(JULE_ERR_FILE_IS_DIR,                     "File is a directory.")                                            \
    _JULE_STATUS_X(JULE_ERR_MMAP_FAILED,                     "mmap() failed.")                                                  \
    _JULE_STATUS_X(JULE_ERR_RELEASE_WHILE_BORROWED,          "Value released while a borrowed reference remains outstanding.")  \
    _JULE_STATUS_X(JULE_ERR_REF_OF_TRANSIENT,                "References may only be taken to non-transient values.")           \
    _JULE_STATUS_X(JULE_ERR_MODIFY_WHILE_ITER,               "Value modified while being iterated.")                            \
    _JULE_STATUS_X(JULE_ERR_LOAD_PACKAGE_FAILURE,            "Failed to load package.")                                         \
    _JULE_STATUS_X(JULE_ERR_USE_PACKAGE_FORBIDDEN,           "use-package has been disabled.")                                  \
    _JULE_STATUS_X(JULE_ERR_ADD_PACKAGE_DIRECTORY_FORBIDDEN, "add-package-directory has been disabled.")                        \
    _JULE_STATUS_X(JULE_ERR_MUST_FOLLOW_IF,                  "This special-form function must follow `if` or `elif`.")

#define _JULE_STATUS_X(e, s) e,
typedef enum { _JULE_STATUS } Jule_Status;
#undef _JULE_STATUS_X

#define _JULE_TYPE                                                           \
    _JULE_TYPE_X(JULE_UNKNOWN,           "<unknown type>")                   \
    _JULE_TYPE_X(JULE_NIL,               "nil")                              \
    _JULE_TYPE_X(JULE_NUMBER,            "number")                           \
    _JULE_TYPE_X(JULE_STRING,            "string")                           \
    _JULE_TYPE_X(JULE_SYMBOL,            "symbol")                           \
    _JULE_TYPE_X(JULE_LIST,              "list")                             \
    _JULE_TYPE_X(JULE_OBJECT,            "object")                           \
    _JULE_TYPE_X(_JULE_REF,               "reference")                       \
    _JULE_TYPE_X(_JULE_TREE,             "unevaluated expression")           \
    _JULE_TYPE_X(_JULE_TREE_LINE_LEADER, "unevaluated expression")           \
    _JULE_TYPE_X(_JULE_FN,               "function")                         \
    _JULE_TYPE_X(_JULE_BUILTIN_FN,       "function (builtin)")               \
    _JULE_TYPE_X(_JULE_LAMBDA,           "lambda")                           \
    _JULE_TYPE_X(_JULE_LIST_OR_OBJECT,   "list or object")                   \
    _JULE_TYPE_X(_JULE_KEYLIKE,          "keylike (string, number, or nil)")

#define _JULE_TYPE_X(e, s) e,
typedef enum { _JULE_TYPE } Jule_Type;
#undef _JULE_TYPE_X

#define JULE_TYPE_IS_KEYLIKE(_t) ((_t) == JULE_STRING || (_t) == JULE_NUMBER || (_t) == JULE_NIL)

enum {
    JULE_NO_QUOTE  = 1u << 0u,
    JULE_MULTILINE = 1u << 1u,
};

typedef void *Jule_Object;

struct Jule_Value_Struct;
typedef struct Jule_Value_Struct Jule_Value;

struct Jule_String_Struct;
typedef struct Jule_String_Struct Jule_String;

typedef const Jule_String *Jule_String_ID;

struct Jule_Array_Struct;
typedef struct Jule_Array_Struct Jule_Array;

struct Jule_Interp_Struct;
typedef struct Jule_Interp_Struct Jule_Interp;

struct Jule_Backtrace_Entry_Struct;
typedef struct Jule_Backtrace_Entry_Struct Jule_Backtrace_Entry;

typedef struct {
    int line;
    int col;
} Jule_Parse_Location;

typedef struct {
    Jule_Interp         *interp;
    Jule_Status          status;
    Jule_Parse_Location  location;
    char                *sym;
    Jule_Type            wanted_type;
    Jule_Type            got_type;
    int                  arity_at_least;
    int                  wanted_arity;
    int                  got_arity;
    Jule_Value          *bad_index;
    char                *file;
    char                *path;
    char                *package_error_message;
} Jule_Error_Info;

typedef void (*Jule_Error_Callback)(Jule_Error_Info *info);
typedef void (*Jule_Output_Callback)(const char*, int);
typedef Jule_Status (*Jule_Eval_Callback)(Jule_Value *value);

typedef Jule_Status (*Jule_Fn)(Jule_Interp*, Jule_Value*, unsigned, Jule_Value**, Jule_Value**);

Jule_Status  jule_map_file_into_readonly_memory(const char *path, const char **addr, int *size);
const char  *jule_error_string(Jule_Status error);
const char  *jule_type_string(Jule_Type type);
char        *jule_to_string(Jule_Interp *interp, const Jule_Value *value, int flags);
Jule_Status  jule_init_interp(Jule_Interp *interp);
Jule_Status  jule_set_error_callback(Jule_Interp *interp, Jule_Error_Callback cb);
Jule_Status  jule_set_output_callback(Jule_Interp *interp, Jule_Output_Callback cb);
Jule_Status  jule_set_eval_callback(Jule_Interp *interp, Jule_Eval_Callback cb);
Jule_Status  jule_set_argv(Jule_Interp *interp, int argc, char **argv);
Jule_Status  jule_load_package(Jule_Interp *interp, const char *name, Jule_Value **result);
void         jule_free_error_info(Jule_Error_Info *info);
Jule_Status  jule_parse(Jule_Interp *interp, const char *str, int size);
Jule_Status  jule_interp(Jule_Interp *interp);
Jule_Value  *jule_nil_value(void);
Jule_Value  *jule_number_value(double num);
Jule_Value  *jule_string_value(Jule_Interp *interp, const char *str);
Jule_Value  *jule_symbol_value(Jule_Interp *interp, const char *symbol);
Jule_Value  *jule_list_value(void);
Jule_Value  *jule_builtin_value(Jule_Fn fn);
Jule_Value  *jule_object_value(void);
Jule_Value  *jule_ref_value(Jule_Value *ref_of);
Jule_Status  jule_insert(Jule_Value *object, Jule_Value *key, Jule_Value *val);
Jule_Status  jule_delete(Jule_Value *object, Jule_Value *key);
Jule_Value  *jule_lookup(Jule_Interp *interp, Jule_String_ID id);
Jule_Status  jule_install_var(Jule_Interp *interp, Jule_String_ID id, Jule_Value *val);
Jule_Status  jule_install_fn(Jule_Interp *interp, Jule_String_ID id, Jule_Fn fn);
Jule_Status  jule_install_local(Jule_Interp *interp, Jule_String_ID id, Jule_Value *val);
Jule_Status  jule_uninstall_var(Jule_Interp *interp, Jule_String_ID id);
Jule_Status  jule_uninstall_fn(Jule_Interp *interp, Jule_String_ID id);
Jule_Status  jule_uninstall_local(Jule_Interp *interp, Jule_String_ID id);
void         jule_free(Jule_Interp *interp);


#ifdef JULE_IMPL

#include <assert.h>

#ifndef JULE_ASSERTIONS
#define JULE_ASSERTIONS (1)
#endif

#if JULE_ASSERTIONS
#define JULE_ASSERT(...) assert(__VA_ARGS__)
#else
#define JULE_ASSERT(...)
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h> /* strlen, memcpy, memset, memcmp */
#include <stdarg.h>
#include <alloca.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>

#ifndef JULE_MALLOC
#define JULE_MALLOC (malloc)
#endif
#ifndef JULE_REALLOC
#define JULE_REALLOC (realloc)
#endif
#ifndef JULE_FREE
#define JULE_FREE (free)
#endif


#define hash_table_make(K_T, V_T, HASH) (CAT2(hash_table(K_T, V_T), _make)((HASH), NULL))
#define hash_table_make_e(K_T, V_T, HASH, EQU) (CAT2(hash_table(K_T, V_T), _make)((HASH), (EQU)))
#define hash_table_len(t) ((t)->len)
#define hash_table_free(t) ((t)->_free((t)))
#define hash_table_get_key(t, k) ((t)->_get_key((t), (k)))
#define hash_table_get_val(t, k) ((t)->_get_val((t), (k)))
#define hash_table_insert(t, k, v) ((t)->_insert((t), (k), (v)))
#define hash_table_delete(t, k) ((t)->_delete((t), (k)))
#define hash_table_traverse(t, key, val_ptr)                         \
    for (/* vars */                                                  \
         uint64_t __i    = 0,                                        \
                  __size = (t)->prime_sizes[(t)->_size_idx];         \
         /* conditions */                                            \
         __i < __size;                                               \
         /* increment */                                             \
         __i += 1)                                                   \
        for (/* vars */                                              \
             __typeof__(*(t)->_data) *__slot_ptr = (t)->_data + __i, \
                                    __slot     = *__slot_ptr;        \
                                                                     \
             /* conditions */                                        \
             __slot != NULL                 &&                       \
             ((key)     = __slot->_key   , 1) &&                     \
             ((val_ptr) = &(__slot->_val), 1);                       \
                                                                     \
             /* increment */                                         \
             __slot_ptr = &(__slot->_next),                          \
             __slot = *__slot_ptr)                                   \
            /* LOOP BODY HERE */                                     \


#define STR(x) _STR(x)
#define _STR(x) #x

#define CAT2(x, y) _CAT2(x, y)
#define _CAT2(x, y) x##y

#define CAT3(x, y, z) _CAT3(x, y, z)
#define _CAT3(x, y, z) x##y##z

#define CAT4(a, b, c, d) _CAT4(a, b, c, d)
#define _CAT4(a, b, c, d) a##b##c##d

#define _hash_table_slot(K_T, V_T) CAT4(_hash_table_slot_, K_T, _, V_T)
#define hash_table_slot(K_T, V_T) CAT4(hash_table_slot_, K_T, _, V_T)
#define _hash_table(K_T, V_T) CAT4(_hash_table_, K_T, _, V_T)
#define hash_table(K_T, V_T) CAT4(hash_table_, K_T, _, V_T)
#define hash_table_pretty_name(K_T, V_T) ("hash_table(" CAT3(K_T, ", ", V_T) ")")

#define _HASH_TABLE_EQU(t_ptr, l, r) \
    ((t_ptr)->_equ ? (t_ptr)->_equ((l), (r)) : (memcmp(&(l), &(r), sizeof((l))) == 0))

#define DEFAULT_START_SIZE_IDX (3)

#define use_hash_table(K_T, V_T)                                                             \
    static uint64_t CAT2(hash_table(K_T, V_T), _prime_sizes)[] = {                           \
        5ULL,        11ULL,        23ULL,        47ULL,        97ULL,                        \
        199ULL,        409ULL,        823ULL,        1741ULL,        3469ULL,                \
        6949ULL,        14033ULL,        28411ULL,        57557ULL,                          \
        116731ULL,        236897ULL,        480881ULL,        976369ULL,                     \
        1982627ULL,        4026031ULL,        8175383ULL,        16601593ULL,                \
        33712729ULL,        68460391ULL,        139022417ULL,                                \
        282312799ULL,        573292817ULL,        1164186217ULL,                             \
        2364114217ULL,        4294967291ULL,        8589934583ULL,                           \
        17179869143ULL,        34359738337ULL,        68719476731ULL,                        \
        137438953447ULL,        274877906899ULL,        549755813881ULL,                     \
        1099511627689ULL,        2199023255531ULL,        4398046511093ULL,                  \
        8796093022151ULL,        17592186044399ULL,        35184372088777ULL,                \
        70368744177643ULL,        140737488355213ULL,                                        \
        281474976710597ULL,        562949953421231ULL,                                       \
        1125899906842597ULL,        2251799813685119ULL,                                     \
        4503599627370449ULL,        9007199254740881ULL,                                     \
        18014398509481951ULL,        36028797018963913ULL,                                   \
        72057594037927931ULL,        144115188075855859ULL,                                  \
        288230376151711717ULL,        576460752303423433ULL,                                 \
        1152921504606846883ULL,        2305843009213693951ULL,                               \
        4611686018427387847ULL,        9223372036854775783ULL,                               \
        18446744073709551557ULL                                                              \
    };                                                                                       \
                                                                                             \
    struct _hash_table(K_T, V_T);                                                            \
                                                                                             \
    typedef struct _hash_table_slot(K_T, V_T) {                                              \
        K_T _key;                                                                            \
        V_T _val;                                                                            \
        uint64_t _hash;                                                                      \
        struct _hash_table_slot(K_T, V_T) *_next;                                            \
    }                                                                                        \
    *hash_table_slot(K_T, V_T);                                                              \
                                                                                             \
    typedef void (*CAT2(hash_table(K_T, V_T), _free_t))                                      \
        (struct _hash_table(K_T, V_T) *);                                                    \
    typedef K_T* (*CAT2(hash_table(K_T, V_T), _get_key_t))                                   \
        (struct _hash_table(K_T, V_T) *, K_T);                                               \
    typedef V_T* (*CAT2(hash_table(K_T, V_T), _get_val_t))                                   \
        (struct _hash_table(K_T, V_T) *, K_T);                                               \
    typedef void (*CAT2(hash_table(K_T, V_T), _insert_t))                                    \
        (struct _hash_table(K_T, V_T) *, K_T, V_T);                                          \
    typedef int (*CAT2(hash_table(K_T, V_T), _delete_t))                                     \
        (struct _hash_table(K_T, V_T) *, K_T);                                               \
    typedef unsigned long long (*CAT2(hash_table(K_T, V_T), _hash_t))(K_T);                  \
    typedef int (*CAT2(hash_table(K_T, V_T), _equ_t))(K_T, K_T);                             \
                                                                                             \
    typedef struct _hash_table(K_T, V_T) {                                                   \
        hash_table_slot(K_T, V_T) *_data;                                                    \
        uint64_t len, _size_idx, _load_thresh;                                               \
        uint64_t *prime_sizes;                                                               \
                                                                                             \
        CAT2(hash_table(K_T, V_T), _free_t)    const _free;                                  \
        CAT2(hash_table(K_T, V_T), _get_key_t) const _get_key;                               \
        CAT2(hash_table(K_T, V_T), _get_val_t) const _get_val;                               \
        CAT2(hash_table(K_T, V_T), _insert_t)  const _insert;                                \
        CAT2(hash_table(K_T, V_T), _delete_t)  const _delete;                                \
        CAT2(hash_table(K_T, V_T), _hash_t)    const _hash;                                  \
        CAT2(hash_table(K_T, V_T), _equ_t)     const _equ;                                   \
    }                                                                                        \
    *hash_table(K_T, V_T);                                                                   \
                                                                                             \
    /* hash_table slot */                                                                    \
    static inline hash_table_slot(K_T, V_T)                                                  \
        CAT2(hash_table_slot(K_T, V_T), _make)(K_T key, V_T val, uint64_t hash) {            \
        hash_table_slot(K_T, V_T) slot = JULE_MALLOC(sizeof(*slot));                         \
                                                                                             \
        slot->_key  = key;                                                                   \
        slot->_val  = val;                                                                   \
        slot->_hash = hash;                                                                  \
        slot->_next = NULL;                                                                  \
                                                                                             \
        return slot;                                                                         \
    }                                                                                        \
                                                                                             \
    /* hash_table */                                                                         \
    static inline void CAT2(hash_table(K_T, V_T), _rehash_insert)                            \
        (hash_table(K_T, V_T) t, hash_table_slot(K_T, V_T) insert_slot) {                    \
                                                                                             \
        uint64_t h, data_size, idx;                                                          \
        hash_table_slot(K_T, V_T) slot, *slot_ptr;                                           \
                                                                                             \
        h         = insert_slot->_hash;                                                      \
        data_size = t->prime_sizes[t->_size_idx];                                            \
        idx       = h % data_size;                                                           \
        slot_ptr  = t->_data + idx;                                                          \
                                                                                             \
        while ((slot = *slot_ptr))    { slot_ptr = &(slot->_next); }                         \
                                                                                             \
        *slot_ptr = insert_slot;                                                             \
    }                                                                                        \
                                                                                             \
    static inline void                                                                       \
        CAT2(hash_table(K_T, V_T), _update_load_thresh)(hash_table(K_T, V_T) t) {            \
                                                                                             \
        uint64_t cur_size;                                                                   \
                                                                                             \
        cur_size        = t->prime_sizes[t->_size_idx];                                      \
        t->_load_thresh = ((double)((cur_size << 1ULL))                                      \
                            / ((double)(cur_size * 3)))                                      \
                            * cur_size;                                                      \
    }                                                                                        \
                                                                                             \
    static inline void CAT2(hash_table(K_T, V_T), _rehash)(hash_table(K_T, V_T) t) {         \
        uint64_t                   old_size,                                                 \
                                   new_data_size;                                            \
        hash_table_slot(K_T, V_T) *old_data,                                                 \
                                   slot,                                                     \
                                  *slot_ptr,                                                 \
                                   next;                                                     \
                                                                                             \
        old_size      = t->prime_sizes[t->_size_idx];                                        \
        old_data      = t->_data;                                                            \
        t->_size_idx += 1;                                                                   \
        new_data_size = sizeof(hash_table_slot(K_T, V_T)) * t->prime_sizes[t->_size_idx];    \
        t->_data      = JULE_MALLOC(new_data_size);                                          \
        memset(t->_data, 0, new_data_size);                                                  \
                                                                                             \
        for (uint64_t i = 0; i < old_size; i += 1) {                                         \
            slot_ptr = old_data + i;                                                         \
            next = *slot_ptr;                                                                \
            while ((slot = next)) {                                                          \
                next        = slot->_next;                                                   \
                slot->_next = NULL;                                                          \
                CAT2(hash_table(K_T, V_T), _rehash_insert)(t, slot);                         \
            }                                                                                \
        }                                                                                    \
                                                                                             \
        JULE_FREE(old_data);                                                                 \
                                                                                             \
        CAT2(hash_table(K_T, V_T), _update_load_thresh)(t);                                  \
    }                                                                                        \
                                                                                             \
    static inline void                                                                       \
        CAT2(hash_table(K_T, V_T), _insert)(hash_table(K_T, V_T) t, K_T key, V_T val) {      \
        uint64_t h, data_size, idx;                                                          \
        hash_table_slot(K_T, V_T) slot, *slot_ptr;                                           \
                                                                                             \
        h         = t->_hash(key);                                                           \
        data_size = t->prime_sizes[t->_size_idx];                                            \
        idx       = h % data_size;                                                           \
        slot_ptr  = t->_data + idx;                                                          \
                                                                                             \
        while ((slot = *slot_ptr)) {                                                         \
            if (_HASH_TABLE_EQU(t, slot->_key, key)) {                                       \
                slot->_val = val;                                                            \
                return;                                                                      \
            }                                                                                \
            slot_ptr = &(slot->_next);                                                       \
        }                                                                                    \
                                                                                             \
        *slot_ptr = CAT2(hash_table_slot(K_T, V_T), _make)(key, val, h);                     \
        t->len   += 1;                                                                       \
                                                                                             \
        if (t->len == t->_load_thresh) {                                                     \
            CAT2(hash_table(K_T, V_T), _rehash)(t);                                          \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    static inline int CAT2(hash_table(K_T, V_T), _delete)                                    \
        (hash_table(K_T, V_T) t, K_T key) {                                                  \
                                                                                             \
        uint64_t h, data_size, idx;                                                          \
        hash_table_slot(K_T, V_T) slot, prev, *slot_ptr;                                     \
                                                                                             \
        h = t->_hash(key);                                                                   \
        data_size = t->prime_sizes[t->_size_idx];                                            \
        idx = h % data_size;                                                                 \
        slot_ptr = t->_data + idx;                                                           \
        prev = NULL;                                                                         \
                                                                                             \
        while ((slot = *slot_ptr)) {                                                         \
            if (_HASH_TABLE_EQU(t, slot->_key, key)) {                                       \
                break;                                                                       \
            }                                                                                \
            prev     = slot;                                                                 \
            slot_ptr = &(slot->_next);                                                       \
        }                                                                                    \
                                                                                             \
        if ((slot = *slot_ptr)) {                                                            \
            if (prev) {                                                                      \
                prev->_next = slot->_next;                                                   \
            } else {                                                                         \
                *slot_ptr = slot->_next;                                                     \
            }                                                                                \
            JULE_FREE(slot);                                                                 \
            t->len -= 1;                                                                     \
            return 1;                                                                        \
        }                                                                                    \
        return 0;                                                                            \
    }                                                                                        \
                                                                                             \
    static inline K_T*                                                                       \
        CAT2(hash_table(K_T, V_T), _get_key)(hash_table(K_T, V_T) t, K_T key) {              \
                                                                                             \
        uint64_t h, data_size, idx;                                                          \
        hash_table_slot(K_T, V_T) slot, *slot_ptr;                                           \
                                                                                             \
        h         = t->_hash(key);                                                           \
        data_size = t->prime_sizes[t->_size_idx];                                            \
        idx       = h % data_size;                                                           \
        slot_ptr  = t->_data + idx;                                                          \
                                                                                             \
        while ((slot = *slot_ptr)) {                                                         \
            if (_HASH_TABLE_EQU(t, slot->_key, key)) {                                       \
                return &slot->_key;                                                          \
            }                                                                                \
            slot_ptr = &(slot->_next);                                                       \
        }                                                                                    \
                                                                                             \
        return NULL;                                                                         \
    }                                                                                        \
                                                                                             \
    static inline V_T*                                                                       \
        CAT2(hash_table(K_T, V_T), _get_val)(hash_table(K_T, V_T) t, K_T key) {              \
                                                                                             \
        uint64_t h, data_size, idx;                                                          \
        hash_table_slot(K_T, V_T) slot, *slot_ptr;                                           \
                                                                                             \
        h         = t->_hash(key);                                                           \
        data_size = t->prime_sizes[t->_size_idx];                                            \
        idx       = h % data_size;                                                           \
        slot_ptr  = t->_data + idx;                                                          \
                                                                                             \
        while ((slot = *slot_ptr)) {                                                         \
            if (_HASH_TABLE_EQU(t, slot->_key, key)) {                                       \
                return &slot->_val;                                                          \
            }                                                                                \
            slot_ptr = &(slot->_next);                                                       \
        }                                                                                    \
                                                                                             \
        return NULL;                                                                         \
    }                                                                                        \
                                                                                             \
    static inline void CAT2(hash_table(K_T, V_T), _free)(hash_table(K_T, V_T) t) {           \
        for (uint64_t i = 0; i < t->prime_sizes[t->_size_idx]; i += 1) {                     \
            hash_table_slot(K_T, V_T) next, slot = t->_data[i];                              \
            while (slot != NULL) {                                                           \
                next = slot->_next;                                                          \
                JULE_FREE(slot);                                                             \
                slot = next;                                                                 \
            }                                                                                \
        }                                                                                    \
        JULE_FREE(t->_data);                                                                 \
        JULE_FREE(t);                                                                        \
    }                                                                                        \
                                                                                             \
    static inline hash_table(K_T, V_T)                                                       \
    CAT2(hash_table(K_T, V_T), _make)(CAT2(hash_table(K_T, V_T), _hash_t) hash,              \
                                      CAT2(hash_table(K_T, V_T), _equ_t)equ) {               \
        hash_table(K_T, V_T) t = JULE_MALLOC(sizeof(*t));                                    \
                                                                                             \
        uint64_t data_size                                                                   \
            =   CAT2(hash_table(K_T, V_T), _prime_sizes)[DEFAULT_START_SIZE_IDX]             \
              * sizeof(hash_table_slot(K_T, V_T));                                           \
        hash_table_slot(K_T, V_T) *the_data = JULE_MALLOC(data_size);                        \
                                                                                             \
        memset(the_data, 0, data_size);                                                      \
                                                                                             \
        struct _hash_table(K_T, V_T)                                                         \
            init                 = {._size_idx = DEFAULT_START_SIZE_IDX,                     \
                    ._data       = the_data,                                                 \
                    .len         = 0,                                                        \
                    .prime_sizes = CAT2(hash_table(K_T, V_T), _prime_sizes),                 \
                    ._free       = CAT2(hash_table(K_T, V_T), _free),                        \
                    ._get_key    = CAT2(hash_table(K_T, V_T), _get_key),                     \
                    ._get_val    = CAT2(hash_table(K_T, V_T), _get_val),                     \
                    ._insert     = CAT2(hash_table(K_T, V_T), _insert),                      \
                    ._delete     = CAT2(hash_table(K_T, V_T), _delete),                      \
                    ._equ        = (CAT2(hash_table(K_T, V_T), _equ_t))equ,                  \
                    ._hash       = (CAT2(hash_table(K_T, V_T), _hash_t))hash};               \
                                                                                             \
        memcpy(t, &init, sizeof(*t));                                                        \
                                                                                             \
        CAT2(hash_table(K_T, V_T), _update_load_thresh)(t);                                  \
                                                                                             \
        return t;                                                                            \
    }                                                                                        \


/* qsort() + a context argument is a total portability mess. Thanks to this guy,
   who wrote a nice wrapper and fallback so that I didn't have to. */

/* Isaac Turner 29 April 2014 Public Domain */

/*

sort_r function to be exported.

Parameters:
  base is the array to be sorted
  nel is the number of elements in the array
  width is the size in bytes of each element of the array
  compar is the comparison function
  arg is a pointer to be passed to the comparison function

void sort_r(void *base, size_t nel, size_t width,
            int (*compar)(const void *_a, const void *_b, void *_arg),
            void *arg);

*/

#define _SORT_R_INLINE inline

#if (defined __APPLE__ || defined __MACH__ || defined __DARWIN__ || \
     (defined __FreeBSD__ && !defined(qsort_r)) || defined __DragonFly__)
#  define _SORT_R_BSD
#elif (defined __GLIBC__ || (defined (__FreeBSD__) && defined(qsort_r)))
#  define _SORT_R_LINUX
#elif (defined _WIN32 || defined _WIN64 || defined __WINDOWS__ || \
       defined __MINGW32__ || defined __MINGW64__)
#  define _SORT_R_WINDOWS
#  undef _SORT_R_INLINE
#  define _SORT_R_INLINE __inline
#else
  /* Using our own recursive quicksort sort_r_simple() */
#endif

#if (defined NESTED_QSORT && NESTED_QSORT == 0)
#  undef NESTED_QSORT
#endif

#define SORT_R_SWAP(a,b,tmp) ((tmp) = (a), (a) = (b), (b) = (tmp))

/* swap a and b */
/* a and b must not be equal! */
static _SORT_R_INLINE void sort_r_swap(char *__restrict a, char *__restrict b,
                                       size_t w)
{
  char tmp, *end = a+w;
  for(; a < end; a++, b++) { SORT_R_SWAP(*a, *b, tmp); }
}

/* swap a, b iff a>b */
/* a and b must not be equal! */
/* __restrict is same as restrict but better support on old machines */
static _SORT_R_INLINE int sort_r_cmpswap(char *__restrict a,
                                         char *__restrict b, size_t w,
                                         int (*compar)(const void *_a,
                                                       const void *_b,
                                                       void *_arg),
                                         void *arg)
{
  if(compar(a, b, arg) > 0) {
    sort_r_swap(a, b, w);
    return 1;
  }
  return 0;
}

/*
Swap consecutive blocks of bytes of size na and nb starting at memory addr ptr,
with the smallest swap so that the blocks are in the opposite order. Blocks may
be internally re-ordered e.g.

  12345ab  ->   ab34512
  123abc   ->   abc123
  12abcde  ->   deabc12
*/
static _SORT_R_INLINE void sort_r_swap_blocks(char *ptr, size_t na, size_t nb)
{
  if(na > 0 && nb > 0) {
    if(na > nb) { sort_r_swap(ptr, ptr+na, nb); }
    else { sort_r_swap(ptr, ptr+nb, na); }
  }
}

/* Implement recursive quicksort ourselves */
/* Note: quicksort is not stable, equivalent values may be swapped */
static _SORT_R_INLINE void sort_r_simple(void *base, size_t nel, size_t w,
                                         int (*compar)(const void *_a,
                                                       const void *_b,
                                                       void *_arg),
                                         void *arg)
{
  char *b = (char *)base, *end = b + nel*w;

  /* for(size_t i=0; i<nel; i++) {printf("%4i", *(int*)(b + i*sizeof(int)));}
  printf("\n"); */

  if(nel < 10) {
    /* Insertion sort for arbitrarily small inputs */
    char *pi, *pj;
    for(pi = b+w; pi < end; pi += w) {
      for(pj = pi; pj > b && sort_r_cmpswap(pj-w,pj,w,compar,arg); pj -= w) {}
    }
  }
  else
  {
    /* nel > 6; Quicksort */

    int cmp;
    char *pl, *ple, *pr, *pre, *pivot;
    char *last = b+w*(nel-1), *tmp;

    /*
    Use median of second, middle and second-last items as pivot.
    First and last may have been swapped with pivot and therefore be extreme
    */
    char *l[3];
    l[0] = b + w;
    l[1] = b+w*(nel/2);
    l[2] = last - w;

    /* printf("pivots: %i, %i, %i\n", *(int*)l[0], *(int*)l[1], *(int*)l[2]); */

    if(compar(l[0],l[1],arg) > 0) { SORT_R_SWAP(l[0], l[1], tmp); }
    if(compar(l[1],l[2],arg) > 0) {
      SORT_R_SWAP(l[1], l[2], tmp);
      if(compar(l[0],l[1],arg) > 0) { SORT_R_SWAP(l[0], l[1], tmp); }
    }

    /* swap mid value (l[1]), and last element to put pivot as last element */
    if(l[1] != last) { sort_r_swap(l[1], last, w); }

    /*
    pl is the next item on the left to be compared to the pivot
    pr is the last item on the right that was compared to the pivot
    ple is the left position to put the next item that equals the pivot
    ple is the last right position where we put an item that equals the pivot

                                           v- end (beyond the array)
      EEEEEELLLLLLLLuuuuuuuuGGGGGGGEEEEEEEE.
      ^- b  ^- ple  ^- pl   ^- pr  ^- pre ^- last (where the pivot is)

    Pivot comparison key:
      E = equal, L = less than, u = unknown, G = greater than, E = equal
    */
    pivot = last;
    ple = pl = b;
    pre = pr = last;

    /*
    Strategy:
    Loop into the list from the left and right at the same time to find:
    - an item on the left that is greater than the pivot
    - an item on the right that is less than the pivot
    Once found, they are swapped and the loop continues.
    Meanwhile items that are equal to the pivot are moved to the edges of the
    array.
    */
    while(pl < pr) {
      /* Move left hand items which are equal to the pivot to the far left.
         break when we find an item that is greater than the pivot */
      for(; pl < pr; pl += w) {
        cmp = compar(pl, pivot, arg);
        if(cmp > 0) { break; }
        else if(cmp == 0) {
          if(ple < pl) { sort_r_swap(ple, pl, w); }
          ple += w;
        }
      }
      /* break if last batch of left hand items were equal to pivot */
      if(pl >= pr) { break; }
      /* Move right hand items which are equal to the pivot to the far right.
         break when we find an item that is less than the pivot */
      for(; pl < pr; ) {
        pr -= w; /* Move right pointer onto an unprocessed item */
        cmp = compar(pr, pivot, arg);
        if(cmp == 0) {
          pre -= w;
          if(pr < pre) { sort_r_swap(pr, pre, w); }
        }
        else if(cmp < 0) {
          if(pl < pr) { sort_r_swap(pl, pr, w); }
          pl += w;
          break;
        }
      }
    }

    pl = pr; /* pr may have gone below pl */

    /*
    Now we need to go from: EEELLLGGGGEEEE
                        to: LLLEEEEEEEGGGG

    Pivot comparison key:
      E = equal, L = less than, u = unknown, G = greater than, E = equal
    */
    sort_r_swap_blocks(b, ple-b, pl-ple);
    sort_r_swap_blocks(pr, pre-pr, end-pre);

    /*for(size_t i=0; i<nel; i++) {printf("%4i", *(int*)(b + i*sizeof(int)));}
    printf("\n");*/

    sort_r_simple(b, (pl-ple)/w, w, compar, arg);
    sort_r_simple(end-(pre-pr), (pre-pr)/w, w, compar, arg);
  }
}


#if defined NESTED_QSORT

  static _SORT_R_INLINE void sort_r(void *base, size_t nel, size_t width,
                                    int (*compar)(const void *_a,
                                                  const void *_b,
                                                  void *aarg),
                                    void *arg)
  {
    int nested_cmp(const void *a, const void *b)
    {
      return compar(a, b, arg);
    }

    qsort(base, nel, width, nested_cmp);
  }

#else /* !NESTED_QSORT */

  /* Declare structs and functions */

  #if defined _SORT_R_BSD

    /* Ensure qsort_r is defined */
    extern void qsort_r(void *base, size_t nel, size_t width, void *thunk,
                        int (*compar)(void *_thunk,
                                      const void *_a, const void *_b));

  #endif

  #if defined _SORT_R_BSD || defined _SORT_R_WINDOWS

    /* BSD (qsort_r), Windows (qsort_s) require argument swap */

    struct sort_r_data
    {
      void *arg;
      int (*compar)(const void *_a, const void *_b, void *_arg);
    };

    static _SORT_R_INLINE int sort_r_arg_swap(void *s,
                                              const void *a, const void *b)
    {
      struct sort_r_data *ss = (struct sort_r_data*)s;
      return (ss->compar)(a, b, ss->arg);
    }

  #endif

  #if defined _SORT_R_LINUX

    typedef int(* __compar_d_fn_t)(const void *, const void *, void *);
    extern void (qsort_r)(void *base, size_t nel, size_t width,
                          __compar_d_fn_t __compar, void *arg)
      __attribute__((nonnull (1, 4)));

  #endif

  /* implementation */

  static _SORT_R_INLINE void sort_r(void *base, size_t nel, size_t width,
                                    int (*compar)(const void *_a,
                                                  const void *_b, void *_arg),
                                    void *arg)
  {
    #if defined _SORT_R_LINUX

      #if defined __GLIBC__ && ((__GLIBC__ < 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 8))

        /* no qsort_r in glibc before 2.8, need to use nested qsort */
        sort_r_simple(base, nel, width, compar, arg);

      #else

        qsort_r(base, nel, width, compar, arg);

      #endif

    #elif defined _SORT_R_BSD

      struct sort_r_data tmp;
      tmp.arg = arg;
      tmp.compar = compar;
      qsort_r(base, nel, width, &tmp, sort_r_arg_swap);

    #elif defined _SORT_R_WINDOWS

      struct sort_r_data tmp;
      tmp.arg = arg;
      tmp.compar = compar;
      qsort_s(base, nel, width, sort_r_arg_swap, &tmp);

    #else

      /* Fall back to our own quicksort implementation */
      sort_r_simple(base, nel, width, compar, arg);

    #endif
  }

#endif /* !NESTED_QSORT */

#undef _SORT_R_INLINE
#undef _SORT_R_WINDOWS
#undef _SORT_R_LINUX
#undef _SORT_R_BSD


Jule_Status jule_map_file_into_readonly_memory(const char *path, const char **addr, int *size) {
    Jule_Status  status;
    FILE        *f;
    int          fd;
    struct stat  fs;

    status = JULE_SUCCESS;
    f      = fopen(path, "r");

    if (f == NULL) { status = JULE_ERR_FILE_NOT_FOUND; goto out; }

    fd = fileno(f);

    if      (fstat(fd, &fs) != 0) { status = JULE_ERR_FILE_NOT_FOUND; goto out_fclose; }
    else if (S_ISDIR(fs.st_mode)) { status = JULE_ERR_FILE_IS_DIR;    goto out_fclose; }

    *size = fs.st_size;

    if (*size == 0) {
        *addr = NULL;
        goto out_fclose;
    }

    *addr = mmap(NULL, *size, PROT_READ, MAP_SHARED, fd, 0);

    if (*addr == MAP_FAILED) { status = JULE_ERR_MMAP_FAILED; goto out_fclose; }

out_fclose:
    fclose(f);

out:
    return status;
}

struct Jule_String_Struct {
    char               *chars;
    unsigned long long  len;
};

static inline char *jule_charptr_ndup(const char *str, int len) {
    char *r;

    r = JULE_MALLOC(len + 1);
    memcpy(r, str, len);
    r[len] = 0;

    return r;
}

static inline char *jule_charptr_dup(const char *str) {
    return jule_charptr_ndup(str, strlen(str));
}

static unsigned long long jule_charptr_hash(char *s) {
    unsigned long hash = 5381;
    int c;

    while ((c = *s++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static int jule_charptr_equ(char *a, char *b) { return strcmp(a, b) == 0; }

static inline void jule_free_string(Jule_String *string) {
    JULE_FREE(string->chars);
    string->chars = NULL;
    string->len   = 0;
}

static inline Jule_String jule_string(const char *s, unsigned long long len) {
    Jule_String string;

    string.len   = len;
    string.chars = JULE_MALLOC(string.len + 1);
    memcpy(string.chars, s, string.len);
    string.chars[string.len] = 0;

    return string;
}

static inline Jule_String jule_string_consume(char *s) {
    Jule_String string;

    string.len   = strlen(s);
    string.chars = s;

    return string;
}

static inline Jule_String jule_strdup(Jule_String *s) {
    return jule_string(s->chars, s->len);
}


struct Jule_Array_Struct {
    unsigned  len;
    unsigned  cap;
    void     *aux;
    void     *data[];
};

#define JULE_ARRAY_INIT        ((Jule_Array*)NULL)
#define JULE_ARRAY_INITIAL_CAP (4)

static inline void jule_free_array(Jule_Array *array) {
    if (array != NULL) { JULE_FREE(array); }
}

static inline unsigned jule_len(Jule_Array *array) {
    return array == NULL ? 0 : array->len;
}

static inline Jule_Array *jule_array_set_aux(Jule_Array *array, void *aux) {
    if (array == NULL) {
        array = JULE_MALLOC(sizeof(Jule_Array) + (JULE_ARRAY_INITIAL_CAP * sizeof(void*)));
        array->len = 0;
        array->cap = JULE_ARRAY_INITIAL_CAP;
    }
    array->aux = aux;
    return array;
}

static inline Jule_Array *jule_push(Jule_Array *array, void *item) {
    if (array == NULL) {
        array = JULE_MALLOC(sizeof(Jule_Array) + (JULE_ARRAY_INITIAL_CAP * sizeof(void*)));
        array->len = 0;
        array->cap = JULE_ARRAY_INITIAL_CAP;
        array->aux = NULL;
        goto push;
    }

    if (array->len >= array->cap) {
        array->cap += ((array->cap >> 1) > 0) ? (array->cap >> 1) : 1;
        array       = JULE_REALLOC(array, sizeof(Jule_Array) + (array->cap * sizeof(void*)));
    }

push:;
    array->data[array->len] = item;
    array->len += 1;

    return array;
}

static inline void *jule_elem(Jule_Array *array, unsigned idx) {
    if (array == NULL || idx >= array->len) {
        return NULL;
    }

    return array->data[idx];
}

static inline void *jule_top(Jule_Array *array) {
    if (array == NULL || array->len == 0) {
        return NULL;
    }

    return array->data[array->len - 1];
}

static inline void *jule_pop(Jule_Array *array) {
    void *r;

    r = NULL;

    if (array != NULL && array->len > 0) {
        r = jule_top(array);
        array->len -= 1;
    }

    return r;
}

static inline void jule_erase(Jule_Array *array, unsigned idx) {
    if (array == NULL || idx >= array->len) {
        return;
    }

    memmove(array->data + idx, array->data + idx + 1, (array->len - idx - 1) * sizeof(*array->data));

    array->len -= 1;
}

#define FOR_EACH(_arrayp, _it)                                                                       \
    for (unsigned _each_i = 0;                                                                       \
         ((_arrayp) != NULL && _each_i < (_arrayp)->len && (((_it) = (_arrayp)->data[_each_i]), 1)); \
         _each_i += 1)



#define JULE_MAX_BORROW_COUNT_POT (10)
#define JULE_MAX_LINE_POT         (17)
#define JULE_MAX_COL_POT          (10)




#define JULE_BORROW(_value)                                                    \
do {                                                                           \
    JULE_ASSERT((_value)->borrow_count < (1u << JULE_MAX_BORROW_COUNT_POT));   \
    (_value)->borrow_count += 1;                                               \
} while (0)

#define JULE_UNBORROW(_value)                                                  \
do {                                                                           \
    JULE_ASSERT((_value)->borrow_count > 0);                                   \
    (_value)->borrow_count -= 1;                                               \
} while (0)

#define JULE_BORROWER(_value)                                                  \
do {                                                                           \
    JULE_ASSERT((_value)->borrower_count < (1u << JULE_MAX_BORROW_COUNT_POT)); \
    (_value)->borrower_count += 1;                                             \
} while (0)

#define JULE_UNBORROWER(_value)                                                \
do {                                                                           \
    JULE_ASSERT((_value)->borrower_count > 0);                                 \
    (_value)->borrower_count -= 1;                                             \
} while (0)



struct Jule_Value_Struct {
    union {
        unsigned long long  _integer;
        double              number;
        Jule_String_ID      string_id;
        Jule_String_ID      symbol_id;
        Jule_Object         object;
        Jule_Array         *list;
        Jule_Array         *eval_values;
        Jule_Fn             builtin_fn;
        Jule_Value         *ref_of;
    };
    unsigned long long      type           :                         4; //  4
    unsigned long long      in_symtab      :                         1; //  5
    unsigned long long      local          :                         1; //  6
    unsigned long long      borrow_count   : JULE_MAX_BORROW_COUNT_POT; // 16
    unsigned long long      borrower_count : JULE_MAX_BORROW_COUNT_POT; // 26
    unsigned long long      is_line_parent :                         1; // 27
    unsigned long long      line           :         JULE_MAX_LINE_POT; // 44
    unsigned long long      col            :          JULE_MAX_COL_POT; // 54
    unsigned long long      ind_level      :          JULE_MAX_COL_POT; // 64
};

typedef struct Jule_Parse_Context_Struct {
    Jule_Interp *interp;
    const char  *cursor;
    const char  *end;
    Jule_Array  *stack;
    Jule_Array  *roots;
    int          line;
    int          col;
    int          ind;
    int          plevel;
} Jule_Parse_Context;


static int jule_equal(Jule_Value *a, Jule_Value *b);

static inline unsigned long long jule_string_id_hash(Jule_String_ID id) {
    return ((unsigned long long)((void*)id)) >> 4;
}

static unsigned long long jule_valhash(Jule_Value *val) {
    JULE_ASSERT(JULE_TYPE_IS_KEYLIKE(val->type));

    /* @todo zeros, nan, inf w/ sign */
    if (val->type == JULE_NUMBER) {
        return val->_integer;
    } else if (val->type == JULE_NIL) {
        return 0;
    }

    return jule_string_id_hash(val->string_id);
}


typedef char *Char_Ptr;

typedef Jule_Value *Jule_Value_Ptr;

use_hash_table(Jule_String_ID, Jule_Value_Ptr)

typedef hash_table(Jule_String_ID, Jule_Value_Ptr) _Jule_Symbol_Table;

use_hash_table(Jule_Value_Ptr, Jule_Value_Ptr)

typedef hash_table(Jule_Value_Ptr, Jule_Value_Ptr) _Jule_Object;

use_hash_table(Char_Ptr, Jule_String_ID)
typedef hash_table(Char_Ptr, Jule_String_ID) _Jule_String_Table;


struct Jule_Interp_Struct {
    Jule_Array            *roots;
    Jule_Error_Callback    error_callback;
    Jule_Output_Callback   output_callback;
    Jule_Eval_Callback     eval_callback;
    _Jule_String_Table     strings;
    _Jule_Symbol_Table     symtab;
    Jule_Array            *local_symtab_stack;
    Jule_Array            *iter_vals;
    Jule_String_ID         cur_file;
    int                    argc;
    char                 **argv;
    int                    use_package_forbidden;
    int                    add_package_directory_forbidden;
    Jule_Array            *package_dirs;
    Jule_Array            *package_handles;
    Jule_Array            *package_values;
    Jule_Array            *backtrace;
    Jule_Fn                last_popped_builtin_fn;
    int                    last_if_was_true;
};

struct Jule_Backtrace_Entry_Struct {
    Jule_String_ID  file;
    Jule_Value     *fn;
};


/* A lambda's eval_values->aux must point to a Jule_Closure_Info. */
typedef struct Jule_Closure_Info_Struct {
    Jule_String_ID     cur_file;
    _Jule_Symbol_Table captures;
} Jule_Closure_Info;


static Jule_String_ID jule_get_string_id(Jule_Interp *interp, const char *s) {
    Jule_String_ID *lookup;
    Jule_String    *newstring;

    lookup = hash_table_get_val(interp->strings, (char*)s);

    if (lookup == NULL) {
        newstring  = JULE_MALLOC(sizeof(*newstring));
        *newstring = jule_string(s, strlen(s));
        hash_table_insert(interp->strings, (*newstring).chars, newstring);
        lookup = hash_table_get_val(interp->strings, (char*)s);
        JULE_ASSERT(lookup != NULL);
    }

    return *lookup;
}

static inline const Jule_String *jule_get_string(Jule_Interp *interp, Jule_String_ID id) {
    (void)interp;
    return id;
}



#define _JULE_STATUS_X(e, s) s,
const char *_jule_error_strings[] = { _JULE_STATUS };
#undef _JULE_STATUS_X

const char *jule_error_string(Jule_Status error) {
    return _jule_error_strings[error];
}

#define _JULE_TYPE_X(e, s) s,
const char *_jule_type_strings[] = { _JULE_TYPE };
#undef _JULE_TYPE_X

const char *jule_type_string(Jule_Type type) {
    return _jule_type_strings[type];
}

Jule_Status jule_set_error_callback(Jule_Interp *interp, Jule_Error_Callback cb) {
    interp->error_callback = cb;
    return JULE_SUCCESS;
}

Jule_Status jule_set_output_callback(Jule_Interp *interp, Jule_Output_Callback cb) {
    interp->output_callback = cb;
    return JULE_SUCCESS;
}

Jule_Status jule_set_eval_callback(Jule_Interp *interp, Jule_Eval_Callback cb) {
    interp->eval_callback = cb;
    return JULE_SUCCESS;
}

Jule_Status jule_set_argv(Jule_Interp *interp, int argc, char **argv) {
    interp->argc = argc;
    interp->argv = argv;
    return JULE_SUCCESS;
}

static Jule_Value *jule_copy_force(Jule_Value *value);

Jule_Status jule_load_package(Jule_Interp *interp, const char *name, Jule_Value **result) {
    Jule_Status         status;
    const Jule_String  *dir;
    unsigned            i;
    char                buff[4096];
    void               *handle;
    unsigned            idx;
    void               *it;
    Jule_Value *      (*init)(Jule_Interp*);
    Jule_Value         *val;

    status = JULE_SUCCESS;

    if (result != NULL) { *result = NULL; }

    snprintf(buff, sizeof(buff), "./%s.so", name);

    if (access(buff, F_OK) >= 0) {
        handle = dlopen(buff, RTLD_LAZY);

        if (handle == NULL) {
            status = JULE_ERR_LOAD_PACKAGE_FAILURE;
            goto out;
        } else {
            goto found_handle;
        }
    }

    if (jule_len(interp->package_dirs) > 0) {
        for (i = jule_len(interp->package_dirs); i > 0; i -= 1) {
            dir = jule_elem(interp->package_dirs, i - 1);

            snprintf(buff, sizeof(buff), "%s/%s.so", dir->chars, name);

            if (access(buff, F_OK) == -1) { continue; }

            handle = dlopen(buff, RTLD_LAZY);

            if (handle == NULL) {
                status = JULE_ERR_LOAD_PACKAGE_FAILURE;
                goto out;
            } else {
                goto found_handle;
            }
        }
    }

    handle = dlopen(buff, RTLD_LAZY); /* get dl to create an error */
    status = JULE_ERR_LOAD_PACKAGE_FAILURE;
    goto out;

found_handle:;
    idx = 0;
    FOR_EACH(interp->package_handles, it) {
        if (it == handle) {
            dlclose(handle);
            if (result != NULL) {
                *result = jule_copy_force(jule_elem(interp->package_values, idx));
            }
            goto out;
        }
        idx += 1;
    }
    *(void**)(&init) = dlsym(handle, "jule_init_package");
    if (init == NULL) {
        *result = NULL;
        status = JULE_ERR_LOAD_PACKAGE_FAILURE;
        goto out;
    }

    val = init(interp);

    interp->package_handles = jule_push(interp->package_handles, handle);
    interp->package_values  = jule_push(interp->package_values,  val);

    if (result != NULL) {
        *result = jule_copy_force(val);
    }

out:;
    return status;
}

Jule_Status jule_add_package_directory(Jule_Interp *interp, const char *path) {
    char *home;
    char  buff[4096];

    if (path[0] == '~') {
        home = getenv("HOME");

        if (home != NULL) {
            snprintf(buff, sizeof(buff), "%s%s", home, path + 1);
            path = buff;
        }
    }

    interp->package_dirs = jule_push(interp->package_dirs, (void*)jule_get_string(interp, jule_get_string_id(interp, path)));

    return JULE_SUCCESS;
}

static void jule_free_value(Jule_Value *value);


void jule_free_error_info(Jule_Error_Info *info) {
    if (info->sym != NULL) {
        JULE_FREE(info->sym);
    }
    if (info->bad_index != NULL) {
        jule_free_value(info->bad_index);
    }
    if (info->file != NULL) {
        JULE_FREE(info->file);
    }
    if (info->path != NULL) {
        JULE_FREE(info->path);
    }
    if (info->package_error_message != NULL) {
        JULE_FREE(info->package_error_message);
    }
}

static inline Jule_Value *_jule_value(void) {
    Jule_Value *value;

    value = JULE_MALLOC(sizeof(*value));
    memset(value, 0, sizeof(*value));

    return value;
}

Jule_Value *jule_nil_value(void) {
    Jule_Value *value;

    value = _jule_value();

    value->type = JULE_NIL;

    return value;
}

Jule_Value *jule_number_value(double num) {
    Jule_Value *value;

    value = _jule_value();

    value->type   = JULE_NUMBER;
    value->number = num;

    return value;
}

Jule_Value *jule_string_value(Jule_Interp *interp, const char *str) {
    Jule_Value *value;

    value = _jule_value();

    value->type      = JULE_STRING;
    value->string_id = jule_get_string_id(interp, str);

    return value;
}

Jule_Value *jule_symbol_value(Jule_Interp *interp, const char *symbol) {
    Jule_Value *value;

    value = _jule_value();

    value->type      = JULE_SYMBOL;
    value->symbol_id = jule_get_string_id(interp, symbol);

    return value;
}

Jule_Value *jule_list_value(void) {
    Jule_Value *value;

    value = _jule_value();

    value->type = JULE_LIST;

    return value;
}

Jule_Value *jule_builtin_value(Jule_Fn fn) {
    Jule_Value *value;

    value = _jule_value();

    value->type       = _JULE_BUILTIN_FN;
    value->builtin_fn = fn;

    return value;
}

Jule_Value *jule_object_value(void) {
    Jule_Value *value;

    value = _jule_value();

    value->type   = JULE_OBJECT;
    value->object = hash_table_make_e(Jule_Value_Ptr, Jule_Value_Ptr, jule_valhash, jule_equal);

    return value;
}

Jule_Value *jule_ref_value(Jule_Value *ref_of) {
    Jule_Value *value;

    value = _jule_value();

    value->type   = _JULE_REF;
    value->ref_of = ref_of;

    JULE_BORROW(ref_of);
    JULE_BORROWER(value);

    return value;
}

static inline int jule_value_is_freeable(Jule_Value *value) {
    return !(value->borrower_count || value->borrow_count || value->in_symtab);
}

static void _jule_free_value(Jule_Value *value, int force) {
    Jule_Value         *child;
    Jule_Value         *key;
    Jule_Value        **val;
    Jule_Closure_Info  *closure;
    Jule_String_ID      sym;

    JULE_ASSERT((!force || !value->borrow_count)
    && "why are we forcing a free of a borrowed value?");

    if (!force && !jule_value_is_freeable(value)) { return; }


    switch (value->type) {
        case JULE_NIL:
            break;
        case JULE_NUMBER:
            break;
        case JULE_STRING:
            break;
        case JULE_SYMBOL:
            break;
        case JULE_LIST:
            FOR_EACH(value->list, child) {
                child->in_symtab = 0;
                _jule_free_value(child, force);
            }
            jule_free_array(value->list);
            break;
        case JULE_OBJECT:
            hash_table_traverse((_Jule_Object)value->object, key, val) {
                key->in_symtab    = 0;
                (*val)->in_symtab = 0;

                _jule_free_value(key, force);
                _jule_free_value(*val, force);
            }
            hash_table_free((_Jule_Object)value->object);
            value->object = NULL;
            break;
        case _JULE_REF:
            JULE_ASSERT(value->borrower_count == 0 && "still marked as a borrower");
            break;
        case _JULE_LAMBDA:
            closure = value->eval_values->aux;
            hash_table_traverse(closure->captures, sym, val) {
                (void)sym;
                _jule_free_value(*val, force);
            }
            hash_table_free(closure->captures);
            JULE_FREE(closure);
            /* fallthrough */
        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
        case _JULE_FN:
            FOR_EACH(value->eval_values, child) {
                child->in_symtab = 0;
                _jule_free_value(child, force);
            }
            jule_free_array(value->eval_values);
            break;
        case _JULE_BUILTIN_FN:
            break;
        default:
            JULE_ASSERT(0);
            break;
    }

    JULE_FREE(value);
}

static void jule_free_value(Jule_Value *value) {
    _jule_free_value(value, 0);
}

static void jule_free_value_force(Jule_Value *value) {
    _jule_free_value(value, 1);
}

static void jule_free_symtab(_Jule_Symbol_Table symtab) {
    Jule_String_ID   key;
    Jule_Value     **val;

    /* Remove all borrowers without freeing the values. They will be freed
     * when the borrowed value is freed below. */
again:;
    hash_table_traverse(symtab, key, val) {
        if ((*val)->type == _JULE_REF) {
            (*val)->borrower_count = 0;
            JULE_UNBORROW((*val)->ref_of);
            jule_free_value_force(*val);
        } else if ((*val)->borrower_count == 0) {
            continue;
        }

        hash_table_delete(symtab, key);
        goto again;
    }

    hash_table_traverse(symtab, key, val) {
        (*val)->in_symtab    = 0;
        (*val)->borrow_count = 0;
        JULE_ASSERT((*val)->borrower_count == 0);
        jule_free_value_force(*val);
    }

    hash_table_free(symtab);
}

Jule_Status jule_insert(Jule_Value *object, Jule_Value *key, Jule_Value *val) {
    Jule_Value **lookup;

    lookup = hash_table_get_val((_Jule_Object)object->object, key);
    if (lookup != NULL) {
        JULE_ASSERT(*lookup != val);

        if ((*lookup)->borrow_count) {
            return JULE_ERR_RELEASE_WHILE_BORROWED;
        }

        jule_free_value(key);
        jule_free_value(*lookup);
        *lookup = val;
    } else {
        hash_table_insert((_Jule_Object)object->object, key, val);
    }

    return JULE_SUCCESS;
}

Jule_Value *jule_field(Jule_Value *object, Jule_Value *key) {
    Jule_Value **lookup;

    lookup = hash_table_get_val((_Jule_Object)object->object, key);
    return lookup == NULL ? NULL : *lookup;
}

Jule_Status jule_delete(Jule_Value *object, Jule_Value *key) {
    Jule_Value **lookup;
    Jule_Value  *real_key;
    Jule_Value  *val;

    lookup = hash_table_get_key((_Jule_Object)object->object, key);

    if (lookup != NULL) {
        real_key = *lookup;
        val      = *hash_table_get_val((_Jule_Object)object->object, real_key);

        if (val->borrow_count) {
            return JULE_ERR_RELEASE_WHILE_BORROWED;
        }

        hash_table_delete((_Jule_Object)object->object, real_key);
        jule_free_value(real_key);
        jule_free_value(val);
    }

    return JULE_SUCCESS;
}

static Jule_Value *_jule_copy(Jule_Value *value, int force) {
    Jule_Value         *copy;
    Jule_Array         *array = JULE_ARRAY_INIT;
    Jule_Value         *child;
    _Jule_Object        obj;
    Jule_Value         *key;
    Jule_Value        **val;
    Jule_Closure_Info  *closure;
    Jule_Closure_Info  *closure_cpy;
    Jule_String_ID      sym;

    if (!force && (value->in_symtab)) { return value; }

    copy = _jule_value();
    memcpy(copy, value, sizeof(*copy));

    switch (value->type) {
        case JULE_NIL:
            break;
        case JULE_NUMBER:
            break;
        case JULE_STRING:
            break;
        case JULE_SYMBOL:
            break;
        case JULE_LIST:
            FOR_EACH(copy->list, child) {
                array = jule_push(array, _jule_copy(child, force));
            }
            copy->list = array;
            break;
        case JULE_OBJECT:
            obj = copy->object;
            copy->object = hash_table_make_e(Jule_Value_Ptr, Jule_Value_Ptr, jule_valhash, jule_equal);
            hash_table_traverse(obj, key, val) {
                hash_table_insert((_Jule_Object)copy->object, _jule_copy(key, force), _jule_copy(*val, force));
            }
            break;
        case _JULE_REF:
            copy = _jule_copy(value->ref_of, force);
            break;
        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
        case _JULE_FN:
        case _JULE_LAMBDA:
            FOR_EACH(copy->eval_values, child) {
                array = jule_push(array, _jule_copy(child, force));
            }
            copy->eval_values = array;
            if (value->type == _JULE_LAMBDA) {
                closure     = value->eval_values->aux;
                closure_cpy = JULE_MALLOC(sizeof(*closure_cpy));

                closure_cpy->cur_file = closure->cur_file;
                closure_cpy->captures = hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash);

                hash_table_traverse(closure->captures, sym, val) {
                    hash_table_insert(closure_cpy->captures, sym, jule_copy_force(*val));
                }
                copy->eval_values = jule_array_set_aux(copy->eval_values, closure_cpy);
            } else {
                copy->eval_values = jule_array_set_aux(copy->eval_values, value->eval_values->aux);
            }
            break;
        case _JULE_BUILTIN_FN:
            break;
        default:
            JULE_ASSERT(0);
            break;
    }

    if (force) {
        copy->in_symtab      = 0;
        copy->local          = 0;
        copy->borrow_count   = 0;
        copy->borrower_count = 0;
    }

    return copy;
}

static Jule_Value *jule_copy(Jule_Value *value) {
    return _jule_copy(value, 0);
}

static Jule_Value *jule_copy_force(Jule_Value *value) {
    return _jule_copy(value, 1);
}

static int jule_equal(Jule_Value *a, Jule_Value *b) {
    unsigned    i;
    Jule_Value *ia;
    Jule_Value *ib;

    if (a->type != b->type) { return 0; }

    switch (a->type) {
        case JULE_NIL:
            return 1;
        case JULE_NUMBER:
            return a->number == b->number;
        case JULE_STRING:
            return a->string_id == b->string_id;
        case JULE_SYMBOL:
            return a->symbol_id == b->symbol_id;
        case JULE_LIST:
            if (jule_len(a->list) != jule_len(b->list)) { return 0; }
            for (i = 0; i < jule_len(a->list); i += 1) {
                ia = jule_elem(a->list, i);
                ib = jule_elem(b->list, i);
                if (!jule_equal(ia, ib)) { return 0; }
            }
            return 1;
        default:
            /* @todo: all types should be covered here */
            JULE_ASSERT(0);
            break;
    }

    return 0;
}

static void jule_error(Jule_Interp *interp, Jule_Error_Info *info) {
    if (interp->error_callback != NULL) {
        interp->error_callback(info);
    } else {
        jule_free_error_info(info);
    }
}

static void jule_make_parse_error(Jule_Interp *interp, int line, int col, Jule_Status status) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = status;
    info.location.line = line;
    info.location.col  = col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    jule_error(interp, &info);
}

static void jule_make_interp_error(Jule_Interp *interp, Jule_Value *value, Jule_Status status) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = status;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    jule_error(interp, &info);
}

static void jule_make_lookup_error(Jule_Interp *interp, Jule_Value *value, Jule_String_ID id) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = JULE_ERR_LOOKUP;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.sym           = jule_charptr_dup(jule_get_string(interp, id)->chars);
    jule_error(interp, &info);
}

static void jule_make_arity_error(Jule_Interp *interp, Jule_Value *value, int wanted, int got, int at_least) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp         = interp;
    info.status         = JULE_ERR_ARITY;
    info.location.line  = value->line;
    info.location.col   = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.wanted_arity   = wanted;
    info.got_arity      = got;
    info.arity_at_least = at_least;
    jule_error(interp, &info);
}

static void jule_make_type_error(Jule_Interp *interp, Jule_Value *value, Jule_Type wanted, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = JULE_ERR_TYPE;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.wanted_type   = wanted;
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_object_key_type_error(Jule_Interp *interp, Jule_Value *value, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = JULE_ERR_OBJECT_KEY_TYPE;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_bad_invoke_error(Jule_Interp *interp, Jule_Value *value, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = JULE_ERR_BAD_INVOKE;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_bad_index_error(Jule_Interp *interp, Jule_Value *value, Jule_Value *bad_index) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = JULE_ERR_BAD_INDEX;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.bad_index     = bad_index;
    jule_error(interp, &info);
}

static void jule_make_file_error(Jule_Interp *interp, Jule_Value *value, Jule_Status status, const char *path) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = status;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.path          = jule_charptr_dup(path);
    jule_error(interp, &info);
}

static void jule_make_install_error(Jule_Interp *interp, Jule_Value *value, Jule_Status status, Jule_String_ID id) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp        = interp;
    info.status        = status;
    info.location.line = value->line;
    info.location.col  = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.sym           = id == NULL ? NULL : jule_charptr_dup(jule_get_string(interp, id)->chars);
    jule_error(interp, &info);
}

static void jule_make_load_package_error(Jule_Interp *interp, Jule_Value *value, Jule_Status status, const char *path, const char *message) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp                = interp;
    info.status                = status;
    info.location.line         = value->line;
    info.location.col          = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    info.path                  = jule_charptr_dup(path);
    info.package_error_message = message == NULL ? "unknown error" : jule_charptr_dup(message);
    jule_error(interp, &info);
}

static void jule_make_forbidden_error(Jule_Interp *interp, Jule_Value *value, Jule_Status status) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp                = interp;
    info.status                = status;
    info.location.line         = value->line;
    info.location.col          = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    jule_error(interp, &info);
}

static void jule_make_must_follow_if_error(Jule_Interp *interp, Jule_Value *value) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.interp                = interp;
    info.status                = JULE_ERR_MUST_FOLLOW_IF;
    info.location.line         = value->line;
    info.location.col          = value->col;
    if (interp->cur_file != NULL) { info.file = jule_charptr_dup(jule_get_string(interp, interp->cur_file)->chars); }
    jule_error(interp, &info);
}

#define STATUS_ERR_RET(_interp, _status)       \
do {                                           \
    if ((_status) != JULE_SUCCESS) {           \
        jule_make_error((_interp), (_status)); \
    }                                          \
    return (_status);                          \
} while (0)

#define PARSE_ERR_RET(_interp, _status, _line, _col)                  \
do {                                                                  \
    if ((_status) != JULE_SUCCESS) {                                  \
        jule_make_parse_error((_interp), (_line), (_col), (_status)); \
    }                                                                 \
    return (_status);                                                 \
} while (0)

static inline int jule_is_space(int c) {
    unsigned char d = c - 9;
    return (0x80001FU >> (d & 31)) & (1U >> (d >> 5));
}

static inline int jule_is_digit(int c) {
    return (unsigned int)(('0' - 1 - c) & (c - ('9' + 1))) >> (sizeof(c) * 8 - 1);
}

static inline int jule_is_alpha(int c) {
    return (unsigned int)(('a' - 1 - (c | 32)) & ((c | 32) - ('z' + 1))) >> (sizeof(c) * 8 - 1);
}

static inline int jule_is_alnum(int c) {
    return jule_is_alpha(c) || jule_is_digit(c);
}

typedef enum {
    JULE_TK_NONE,
    JULE_TK_LPAREN,
    JULE_TK_RPAREN,
    JULE_TK_SYMBOL,
    JULE_TK_NUMBER,
    JULE_TK_STRING,
    JULE_TK_EOS_ERR,
} Jule_Token;

#define MORE_INPUT(_cxt)    ((_cxt)->cursor < (_cxt)->end)
#define PEEK_CHAR(_cxt, _c) ((_c) = (MORE_INPUT(_cxt) ? (*(_cxt)->cursor) : 0))
#define NEXT(_cxt)          ((_cxt)->cursor += 1)
#define SPC(_c)             (jule_is_space(_c))
#define DIG(_c)             (jule_is_digit(_c))

static Jule_Token jule_parse_token(Jule_Parse_Context *cxt) {
    int         c;
    int         last;
    const char *start;

    if (!PEEK_CHAR(cxt, c)) { return JULE_TK_NONE; }

    if (c == '(') {
        NEXT(cxt);
        return JULE_TK_LPAREN;
    } else if (c == ')') {
        NEXT(cxt);
        return JULE_TK_RPAREN;
    } else if (c == '"') {
        do {
            if (c == '\n') { return JULE_TK_EOS_ERR; }
            last = c;
            NEXT(cxt);
        } while (PEEK_CHAR(cxt, c) && (c != '"' || last == '\\'));

        NEXT(cxt);

        return JULE_TK_STRING;
    } else if (c == '-' && ((cxt->cursor + 1) < cxt->end) && DIG(*(cxt->cursor + 1))) {
        NEXT(cxt);
        goto digits;

    } else if (DIG(c)) {
digits:;
        while (PEEK_CHAR(cxt, c) && DIG(c)) { NEXT(cxt); }
        if (PEEK_CHAR(cxt, c) == '.') {
            NEXT(cxt);
            while (PEEK_CHAR(cxt, c) && DIG(c)) { NEXT(cxt); }
        }

        return JULE_TK_NUMBER;
    }

    start = cxt->cursor;

    while (PEEK_CHAR(cxt, c)
    &&     !SPC(c)
    &&     c != '#'
    &&     c != '('
    &&     c != ')') {

        NEXT(cxt);
    }

    if (cxt->cursor > start) {
        return JULE_TK_SYMBOL;
    }

    return JULE_TK_NONE;
}

static int jule_trim_leading_ws(Jule_Parse_Context *cxt) {
    int w;
    int c;

    w = 0;

    while (PEEK_CHAR(cxt, c) && c != '\n' && SPC(c)) {
        NEXT(cxt);
        w += 1;
    }

    return w;
}

static void jule_push_tree(Jule_Parse_Context *cxt) {
    Jule_Value *value;

    value = _jule_value();
    value->type        = _JULE_TREE;
    value->ind_level   = cxt->ind;
    value->line        = cxt->line;
    value->col         = cxt->col;
    value->eval_values = JULE_ARRAY_INIT;
    value->eval_values = jule_array_set_aux(value->eval_values, (void*)cxt->interp->cur_file);

    cxt->stack = jule_push(cxt->stack, value);
}

static void jule_ensure_top_is_line_leader(Jule_Parse_Context *cxt) {
    Jule_Value *top;
    Jule_Value *value;

    top = jule_top(cxt->stack);

    if (top->type == _JULE_TREE_LINE_LEADER) { return; }

    value = _jule_value();
    memcpy(value, top, sizeof(*value));
    memset(top, 0, sizeof(*top));
    top->type        = _JULE_TREE_LINE_LEADER;
    top->ind_level   = value->ind_level;
    top->line        = value->line;
    top->col         = value->col;
    top->eval_values = jule_push(top->eval_values, value);
    top->eval_values = jule_array_set_aux(top->eval_values, (void*)cxt->interp->cur_file);
}

static int jule_consume_comment(Jule_Parse_Context *cxt) {
    int c;

    if (PEEK_CHAR(cxt, c) && c == '#') {
        NEXT(cxt);
        while (PEEK_CHAR(cxt, c)) {
            if (c == '\n') { break; }
            NEXT(cxt);
        }
        return 1;
    }

    return 0;
}

static Jule_Status jule_parse_next_value(Jule_Parse_Context *cxt, Jule_Value **valout, Jule_Token *tkout) {
    int                 status;
    Jule_Value         *val;
    int                 start_col;
    const char         *tk_start;
    Jule_Token          tk;
    const char         *tk_end;
    Jule_Value         *top;
    Jule_Value         *child;
    int                 c;
    char               *sbuff;
    unsigned long long  slen;
    char                d_copy[128];
    double              d;

    status  = JULE_SUCCESS;
    val     = NULL;
    *valout = NULL;
    *tkout  = JULE_TK_NONE;

    cxt->col += jule_trim_leading_ws(cxt);
    if (jule_consume_comment(cxt)) { goto out; }

    tk_start = cxt->cursor;
    if ((tk = jule_parse_token(cxt)) == JULE_TK_NONE) { goto out; }
    tk_end = cxt->cursor;

    if (cxt->col >= (1 << JULE_MAX_COL_POT)) {
        PARSE_ERR_RET(cxt->interp, JULE_ERR_LINE_TOO_LONG, cxt->line, cxt->col);
    }

    start_col = cxt->col;

    if (tk == JULE_TK_LPAREN) {
        jule_push_tree(cxt);
        val = top = jule_top(cxt->stack);

        cxt->col += tk_end - tk_start;

        cxt->plevel += 1;

        child = NULL;
        while ((status = jule_parse_next_value(cxt, &child, tkout)) == JULE_SUCCESS && child != NULL) {
            top->eval_values = jule_push(top->eval_values, child);
        }

        if (status != JULE_SUCCESS) {
            PARSE_ERR_RET(cxt->interp, status, cxt->line, cxt->col);
        }

        if (*tkout != JULE_TK_RPAREN) {
            PARSE_ERR_RET(cxt->interp, JULE_ERR_MISSING_RPAREN, cxt->line, cxt->col);
        }

        if (jule_len(top->eval_values) == 0) {
            PARSE_ERR_RET(cxt->interp, JULE_ERR_EMPTY_PARENS, cxt->line, cxt->col - 1);
        }

        cxt->plevel -= 1;

        *tkout = JULE_TK_LPAREN;

        jule_pop(cxt->stack);

        goto out_val;
    } else if (tk == JULE_TK_RPAREN) {
        if (cxt->plevel <= 0) {
            PARSE_ERR_RET(cxt->interp, JULE_ERR_EXTRA_RPAREN, cxt->line, cxt->col);
        }

        *tkout = JULE_TK_RPAREN;

        cxt->col += tk_end - tk_start;
        goto out;
    }

    *tkout = tk;

    cxt->col += tk_end - tk_start;

    switch (tk) {
        case JULE_TK_SYMBOL:
            if (tk_end - tk_start == 3 && strncmp(tk_start, "nil", tk_end - tk_start) == 0) {
                val = jule_nil_value();
            } else {
                sbuff = alloca(tk_end - tk_start + 1);
                memcpy(sbuff, tk_start, tk_end - tk_start);
                sbuff[tk_end - tk_start] = 0;
                val = jule_symbol_value(cxt->interp, sbuff);
            }
            break;
        case JULE_TK_STRING:
            JULE_ASSERT(tk_start[0] == '"' && "string doesn't start with quote");
            tk_start += 1;

            sbuff = alloca(tk_end - tk_start + 1);
            slen  = 0;

            for (; tk_start < tk_end; tk_start += 1) {
                c = *tk_start;

                if (c == '"') { break; }
                if (c == '\\') {
                    tk_start += 1;
                    if (tk_start < tk_end) {
                        switch (*tk_start) {
                            case '\\':
                                break;
                            case 'n':
                                c = '\n';
                                break;
                            case 'r':
                                c = '\r';
                                break;
                            case 't':
                                c = '\t';
                                break;
                            case '"':
                                c = '"';
                                break;
                            default:
                                sbuff[slen]  = c;
                                slen        += 1;
                                c            = *tk_start;
                                goto add_char;
                        }
                    }
                    goto add_char;
                } else {
add_char:;
                    sbuff[slen] = c;
                }
                slen += 1;
            }

            sbuff[slen] = 0;

            val = jule_string_value(cxt->interp, sbuff);
            break;
        case JULE_TK_NUMBER:
            strncpy(d_copy, tk_start, tk_end - tk_start);
            d_copy[tk_end - tk_start] = 0;
            sscanf(d_copy, "%lg", &d);
            val = jule_number_value(d);
            break;
        case JULE_TK_EOS_ERR:
            PARSE_ERR_RET(cxt->interp, JULE_ERR_UNEXPECTED_EOS, cxt->line, start_col + (tk_end - tk_start));
            break;
        default:
            break;
    }

out_val:;

    JULE_ASSERT(val != NULL);

    val->ind_level = cxt->ind;
    val->line      = cxt->line;
    val->col       = start_col;

    *valout = val;

out:;
    return status;
}

static Jule_Status jule_parse_line(Jule_Parse_Context *cxt) {
    int         status;
    int         c;
    Jule_Value *top;
    int         first;
    Jule_Value *val;
    Jule_Token  tk;

    status = JULE_SUCCESS;

    cxt->ind = jule_trim_leading_ws(cxt);
    cxt->col = 1 + cxt->ind;
    first    = 1;

    if (!PEEK_CHAR(cxt, c))                     { goto done; }
    if (c == '\n' || jule_consume_comment(cxt)) { goto eol;  }

    while ((top = jule_top(cxt->stack)) != NULL
    &&     cxt->ind <= top->ind_level) {

        jule_pop(cxt->stack);
    }

    val = NULL;
    while ((status = jule_parse_next_value(cxt, &val, &tk)) == JULE_SUCCESS && val != NULL) {
        if (first) {
            if (top != NULL) {
                jule_ensure_top_is_line_leader(cxt);
                top->eval_values = jule_push(top->eval_values, val);
            } else {
                cxt->roots = jule_push(cxt->roots, val);
            }
            cxt->stack = jule_push(cxt->stack, val);
            top = val;
        } else {
            jule_ensure_top_is_line_leader(cxt);
            top->eval_values = jule_push(top->eval_values, val);
        }

        first = 0;
    }

    if (status != JULE_SUCCESS) {
        PARSE_ERR_RET(cxt->interp, status, cxt->line, cxt->col);
    }

eol:;
    if (PEEK_CHAR(cxt, c)) {
        if (c == '\n') {
            NEXT(cxt);
        } else {
            PARSE_ERR_RET(cxt->interp, JULE_ERR_UNEXPECTED_TOK, cxt->line, cxt->col);
        }
    }

done:;
    return status;
}

static void _jule_string_print(Jule_Interp *interp, char **buff, int *len, int *cap, const Jule_Value *value, unsigned ind, int flags) {
    unsigned            i;
    char                b[128];
    const Jule_String  *string;
    Jule_Value         *child;
    Jule_Value         *key;
    Jule_Value        **val;
    Jule_String_ID      sym;
    Jule_String_ID      fsym;
    union {
        Jule_Fn         f;
        void           *v;
    }                   prfn;

#define PUSHC(_c)                               \
do {                                            \
    if (*len == *cap) {                         \
        *cap <<= 1;                             \
        *buff = JULE_REALLOC(*buff, *cap);      \
    }                                           \
    (*buff)[*len]  = (_c);                      \
    *len        += 1;                           \
} while (0)

#define PUSHSN(_s, _n)                          \
do {                                            \
    for (unsigned _i = 0; _i < (_n); _i += 1) { \
        PUSHC((_s)[_i]);                        \
    }                                           \
} while (0)

#define PUSHS(_s) PUSHSN((_s), strlen(_s))

    if ((value->type != _JULE_TREE) && (value->type != _JULE_TREE_LINE_LEADER)) {
        for (i = 0; i < ind; i += 1) { PUSHC(' '); }
    }

    switch (value->type) {
        case JULE_NIL:
            PUSHS("nil");
            break;
        case JULE_NUMBER:
            snprintf(b, sizeof(b), "%g", value->number);
            PUSHS(b);
            break;
        case JULE_STRING:
            string = jule_get_string(interp, value->string_id);
            if (flags & JULE_NO_QUOTE) {
                PUSHSN(string->chars, string->len);
            } else {
                PUSHC('"');
                PUSHSN(string->chars, string->len);
                PUSHC('"');
            }
            break;
        case JULE_SYMBOL:
            string = jule_get_string(interp, value->symbol_id);
            PUSHS(string->chars);
            break;
        case JULE_LIST:
            PUSHC('[');
            PUSHC((flags & JULE_MULTILINE) ? '\n' : ' ');
            FOR_EACH(value->list, child) {
                _jule_string_print(interp, buff, len, cap, child, (flags & JULE_MULTILINE) ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC((flags & JULE_MULTILINE) ? '\n' : ' ');
            }
            if (flags & JULE_MULTILINE) {
                for (i = 0; i < ind; i += 1) { PUSHC(' '); }
            }
            PUSHC(']');
            break;
        case JULE_OBJECT:
            PUSHC('{');
            PUSHC((flags & JULE_MULTILINE) ? '\n' : ' ');
            hash_table_traverse((_Jule_Object)value->object, key, val) {
                _jule_string_print(interp, buff, len, cap, key, (flags & JULE_MULTILINE) ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC(':');
                _jule_string_print(interp, buff, len, cap, *val, (flags & JULE_MULTILINE) ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC((flags & JULE_MULTILINE) ? '\n' : ' ');
            }
            if (flags & JULE_MULTILINE) {
                for (i = 0; i < ind; i += 1) { PUSHC(' '); }
            }
            PUSHC('}');
            break;
        case _JULE_FN:
            fsym = NULL;

            for (i = jule_len(interp->local_symtab_stack); i > 0; i -= 1) {
                hash_table_traverse((_Jule_Symbol_Table)jule_elem(interp->local_symtab_stack, i - 1), sym, val) {
                    if ((*val) == value) {
                        fsym = sym;
                        goto found_fsym;
                    }
                }
            }
            hash_table_traverse(interp->symtab, sym, val) {
                if ((*val) == value) {
                    fsym = sym;
                    goto found_fsym;
                }
            }
            goto print_tree; /* Not sure that this could ever happen, but just to be safe. */
found_fsym:;
            snprintf(b, sizeof(b), "<fn@%p> %s", (void*)value, fsym->chars);
            PUSHS(b);
            break;
        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
        case _JULE_LAMBDA:
print_tree:;
            if (flags & JULE_MULTILINE) {
                _jule_string_print(interp, buff, len, cap, value->eval_values->data[0], ind, flags & ~JULE_NO_QUOTE);
                for (i = 1; i < jule_len(value->eval_values); i += 1) {
                    PUSHC('\n');
                    _jule_string_print(interp, buff, len, cap, value->eval_values->data[i], ind + 2, flags & ~JULE_NO_QUOTE);
                }
            } else {
                PUSHC('(');
                _jule_string_print(interp, buff, len, cap, value->eval_values->data[0], ind, flags & ~JULE_NO_QUOTE);
                for (i = 1; i < jule_len(value->eval_values); i += 1) {
                    PUSHC(' ');
                    _jule_string_print(interp, buff, len, cap, value->eval_values->data[i], 0, flags & ~JULE_NO_QUOTE);
                }
                PUSHC(')');
            }
            break;
        case _JULE_BUILTIN_FN:
            fsym = NULL;
            hash_table_traverse(interp->symtab, sym, val) {
                if ((*val)->type == _JULE_BUILTIN_FN
                &&  (*val)->builtin_fn == value->builtin_fn) {

                    fsym = sym;
                    break;
                }
            }
            prfn.f = value->builtin_fn;
            if (fsym != NULL) {
                snprintf(b, sizeof(b), "<fn@%p> %s", prfn.v, fsym->chars);
            } else {
                snprintf(b, sizeof(b), "<fn@%p>", prfn.v);
            }
            PUSHS(b);
            break;
        default:
            JULE_ASSERT(0);
            break;

    }

    PUSHC(0);
    *len -= 1;
}

char *jule_to_string(Jule_Interp *interp, const Jule_Value *value, int flags) {
    char *buff;
    int   len;
    int   cap;

    buff = JULE_MALLOC(16);
    len  = 0;
    cap  = 16;

    _jule_string_print(interp, &buff, &len, &cap, value, 0, flags);

    return buff;
}

static void jule_output(Jule_Interp *interp, const char *s, int n_bytes) {
    if (interp->output_callback == NULL) {
        printf("%.*s", n_bytes, s);
        fflush(stdout);
    } else {
        interp->output_callback(s, n_bytes);
    }
}

static void jule_print(Jule_Interp *interp, Jule_Value *value, unsigned ind) {
    char *buff;
    int   len;
    int   cap;

    buff = JULE_MALLOC(16);
    len  = 0;
    cap  = 16;

    _jule_string_print(interp, &buff, &len, &cap, value, ind, JULE_NO_QUOTE | JULE_MULTILINE);
    jule_output(interp, buff, len);
    JULE_FREE(buff);
}

static Jule_Status jule_parse_nodes(Jule_Interp *interp, const char *str, int size, Jule_Array **out_nodes) {
    Jule_Parse_Context  cxt;
    Jule_Status         status;
    Jule_Value         *it;

    memset(&cxt, 0, sizeof(cxt));

    cxt.interp = interp;
    cxt.cursor = str;
    cxt.end    = str + size;

    status = JULE_SUCCESS;
    while (status == JULE_SUCCESS && MORE_INPUT(&cxt)) {
        cxt.line += 1;

        if (cxt.line >= (1 << JULE_MAX_LINE_POT)) {
            PARSE_ERR_RET(cxt.interp, JULE_ERR_TOO_MANY_LINES, cxt.line, 1);
        }

        status = jule_parse_line(&cxt);
    }

    FOR_EACH(cxt.roots, it) {
        *out_nodes = jule_push(*out_nodes, it);
    }

    jule_free_array(cxt.roots);
    jule_free_array(cxt.stack);

    return status;
}

Jule_Status jule_parse(Jule_Interp *interp, const char *str, int size) {
    return jule_parse_nodes(interp, str, size, &interp->roots);
}

static void jule_pushlocal_symtab(Jule_Interp *interp, _Jule_Symbol_Table local_symtab) {
    interp->local_symtab_stack = jule_push(interp->local_symtab_stack, local_symtab);
}

static Jule_Status jule_pop_local_symtab(Jule_Interp *interp, Jule_Value *tree) {
    Jule_Status          status;
    _Jule_Symbol_Table   local_symtab;
    Jule_Array          *syms = JULE_ARRAY_INIT;
    Jule_String_ID       key;
    Jule_Value         **vit;

    JULE_ASSERT(jule_len(interp->local_symtab_stack) > 0);

    status = JULE_SUCCESS;

    local_symtab = jule_top(interp->local_symtab_stack);

    syms = JULE_ARRAY_INIT;
    hash_table_traverse(local_symtab, key, vit) {
        (void)vit;
        syms = jule_push(syms, (void*)key);
    }
    FOR_EACH(syms, key) {
        status = jule_uninstall_local(interp, key);
        if (status != JULE_SUCCESS) {
            jule_make_install_error(interp, tree, status, key);
            goto out;
        }
    }
    jule_free_array(syms);
    hash_table_free(local_symtab);

    jule_pop(interp->local_symtab_stack);

out:;
    return status;
}

static _Jule_Symbol_Table jule_local_symtab(Jule_Interp *interp) {
    return jule_top(interp->local_symtab_stack);
}

Jule_Value *jule_lookup(Jule_Interp *interp, Jule_String_ID id) {
    Jule_Value **lookup;
    Jule_Value  *val;

    lookup = hash_table_get_val(jule_local_symtab(interp), id);

    if (lookup == NULL) {
        lookup = hash_table_get_val(interp->symtab, id);
    }

    if (lookup == NULL) { return NULL; }

    val = *lookup;

    if (val->type == _JULE_REF) {
        val = val->ref_of;
    }

    return val;
}

Jule_Value *jule_lookup_local_only(Jule_Interp *interp, Jule_String_ID id) {
    Jule_Value **lookup;

    lookup = hash_table_get_val(jule_local_symtab(interp), id);

    return lookup == NULL ? NULL : *lookup;
}

static Jule_Status jule_install_common(Jule_Interp *interp, _Jule_Symbol_Table symtab, Jule_String_ID id, Jule_Value *val, int local) {
    Jule_Value **lookup;

    (void)interp;

    JULE_ASSERT(val->borrower_count || !val->in_symtab);

    lookup = hash_table_get_val(symtab, id);
    if (lookup != NULL) {
        if (*lookup != val) {
            if ((*lookup)->type == _JULE_REF) {
                JULE_UNBORROWER((*lookup));
                JULE_UNBORROW((*lookup)->ref_of);
                (*lookup)->in_symtab = 0;
                jule_free_value(*lookup);
            } else if (!(*lookup)->borrower_count) {
                if ((*lookup)->borrow_count) {
                    return JULE_ERR_RELEASE_WHILE_BORROWED;
                }
                (*lookup)->in_symtab = 0;
                jule_free_value(*lookup);
            }

            val->in_symtab = 1;
            val->local     = !!local;
            *lookup = val;
        }
    } else {
        val->in_symtab = 1;
        val->local     = !!local;

        hash_table_insert(symtab, id, val);
    }

    return JULE_SUCCESS;
}

static Jule_Status jule_uninstall_common(Jule_Interp *interp, _Jule_Symbol_Table symtab, Jule_String_ID id, int do_free) {
    Jule_Value **lookup;
    Jule_Value  *val;

    (void)interp;

    lookup = hash_table_get_val(symtab, id);
    if (lookup == NULL) {
        return JULE_ERR_LOOKUP;
    }

    val = *lookup;

    hash_table_delete(symtab, id);


    do_free = do_free && (val->type == _JULE_REF || val->borrower_count == 0);

    if (val->type == _JULE_REF) {
        JULE_UNBORROWER(val);
        JULE_UNBORROW(val->ref_of);
    }

    if (do_free) {
        if (val->borrow_count) {
            return JULE_ERR_RELEASE_WHILE_BORROWED;
        }

        val->in_symtab = 0;
        jule_free_value(val);
    } else {
        val->in_symtab = 0;
    }

    return JULE_SUCCESS;
}

Jule_Status jule_install_var(Jule_Interp *interp, Jule_String_ID id, Jule_Value *val) {
    return jule_install_common(interp, interp->symtab, id, val, 0);
}

Jule_Status jule_install_fn(Jule_Interp *interp, Jule_String_ID id, Jule_Fn fn) {
    return jule_install_var(interp, id, jule_builtin_value(fn));
}

Jule_Status jule_install_local(Jule_Interp *interp, Jule_String_ID id, Jule_Value *val) {
    return jule_install_common(interp, jule_local_symtab(interp), id, val, 1);
}

Jule_Status jule_uninstall_var(Jule_Interp *interp, Jule_String_ID id) {
    return jule_uninstall_common(interp, interp->symtab, id, 1);
}

Jule_Status jule_uninstall_var_no_free(Jule_Interp *interp, Jule_String_ID id) {
    return jule_uninstall_common(interp, interp->symtab, id, 0);
}

Jule_Status jule_uninstall_fn(Jule_Interp *interp, Jule_String_ID id) {
    return jule_uninstall_var(interp, id);
}

Jule_Status jule_uninstall_local(Jule_Interp *interp, Jule_String_ID id) {
    return jule_uninstall_common(interp, jule_local_symtab(interp), id, 1);
}

Jule_Status jule_uninstall_local_no_free(Jule_Interp *interp, Jule_String_ID id) {
    return jule_uninstall_common(interp, jule_local_symtab(interp), id, 0);
}

static Jule_Status jule_eval(Jule_Interp *interp, Jule_Value *value, Jule_Value **result);
static Jule_Status jule_builtin_elem(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result);
static Jule_Status jule_builtin_field(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result);

static Jule_Status jule_invoke(Jule_Interp *interp, Jule_Value *tree, Jule_Value *fn, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status               status;
    Jule_String_ID            save_file;
    Jule_Backtrace_Entry     *bt_entry;
    Jule_Value               *ev;
    Jule_Value               *def_tree;
    Jule_Value               *fn_sym;
    _Jule_Symbol_Table        local_symtab;
    unsigned                  i;
    unsigned                  n_params;
    Jule_Value              **params;
    Jule_Value               *arg_sym;
    Jule_Value               *arg_val;
    Jule_Value               *expr;
    unsigned                  lambda_params;
    const Jule_Closure_Info  *closure;
    Jule_String_ID            cap_sym;
    Jule_Value              **cap_valp;
    Jule_Value               *cap_val;
    Jule_Value                builtin;
    Jule_Backtrace_Entry     *container_bt_entry;
    Jule_Value              **container_args;

    status = JULE_SUCCESS;


    save_file = interp->cur_file;

    bt_entry = JULE_MALLOC(sizeof(*bt_entry));

    bt_entry->file = interp->cur_file;
    bt_entry->fn   = (fn->type == JULE_LIST || fn->type == JULE_OBJECT)
                        ? tree
                        : fn;

    interp->backtrace = jule_push(interp->backtrace, bt_entry);

    if (fn->type == _JULE_TREE || fn->type == _JULE_TREE_LINE_LEADER) {
        interp->cur_file = fn->eval_values->aux;

        status = jule_eval(interp, tree, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
        *result = ev;
    } else if (fn->type == _JULE_FN) {
        interp->cur_file = fn->eval_values->aux;

        def_tree = jule_elem(fn->eval_values, 1);

        if (def_tree->type == _JULE_TREE || def_tree->type == _JULE_TREE_LINE_LEADER) {
            n_params = jule_len(def_tree->eval_values) - 1;
            params   = (Jule_Value**)def_tree->eval_values->data + 1;

            fn_sym = jule_elem(def_tree->eval_values, 0);
        } else if (def_tree->type == JULE_SYMBOL) {
            n_params = 0;
            params   = NULL;

            fn_sym = def_tree;
        } else {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, def_tree, JULE_SYMBOL, def_tree->type);
            *result = NULL;
            goto out;
        }

        if (n_values != n_params) {
            status = JULE_ERR_ARITY;
            jule_make_arity_error(interp, tree, n_params, n_values, 0);
            *result = NULL;
            goto out;
        }

        local_symtab = hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash);

        fn = jule_copy_force(fn);
        JULE_BORROW(fn);
        status = jule_install_common(interp, local_symtab, fn_sym->symbol_id, fn, 1);

        if (status != JULE_SUCCESS) {
            *result = NULL;
            jule_make_install_error(interp, fn_sym, status, fn_sym->symbol_id);
            goto out;
        }

        for (i = 0; i < n_params; i += 1) {
            arg_sym = params[i];
            JULE_ASSERT(arg_sym->type == JULE_SYMBOL);

            status = jule_eval(interp, values[i], &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out;
            }

            arg_val = jule_copy_force(ev);
            jule_free_value(ev);

            status = jule_install_common(interp, local_symtab, arg_sym->symbol_id, arg_val, 1);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                jule_make_install_error(interp, arg_val, status, arg_sym->symbol_id);
                goto out;
            }
        }

        jule_pushlocal_symtab(interp, local_symtab);

        for (i = 2; i < jule_len(fn->eval_values); i += 1) {
            expr   = jule_elem(fn->eval_values, i);
            status = jule_eval(interp, expr, &ev);
            if (status != JULE_SUCCESS) {
                jule_pop(interp->local_symtab_stack);
                jule_free_symtab(local_symtab);
                *result = NULL;
                goto out;
            }
            if (i == jule_len(fn->eval_values) - 1) {
                *result = ev;
            } else {
                jule_free_value(ev);
            }
        }

        if ((*result)->local) {
            *result = jule_copy_force(*result);
        }

        JULE_UNBORROW(fn);

        status = jule_pop_local_symtab(interp, tree);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
    } else if (fn->type == _JULE_BUILTIN_FN) {
        status = fn->builtin_fn(interp, tree, n_values, values, result);
    } else if (fn->type == _JULE_LAMBDA) {
        closure          = fn->eval_values->aux;
        interp->cur_file = closure->cur_file;

        lambda_params = jule_len(fn->eval_values) > 2;

        if (lambda_params) {
            def_tree = jule_elem(fn->eval_values, 1);

            if (def_tree->type == _JULE_TREE || def_tree->type == _JULE_TREE_LINE_LEADER) {
                n_params = jule_len(def_tree->eval_values);
                params   = (Jule_Value**)def_tree->eval_values->data;
            } else {
                status = JULE_ERR_TYPE;
                jule_make_type_error(interp, def_tree, JULE_SYMBOL, def_tree->type);
                *result = NULL;
                goto out;
            }

            if (n_values != n_params) {
                status = JULE_ERR_ARITY;
                jule_make_arity_error(interp, tree, n_params, n_values, 0);
                *result = NULL;
                goto out;
            }
        } else {
            n_params = 0;
            params   = NULL;
        }

        local_symtab = hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash);

        hash_table_traverse(closure->captures, cap_sym, cap_valp) {
            cap_val = jule_copy_force(*cap_valp);
            status  = jule_install_common(interp, local_symtab, cap_sym, cap_val, 1);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                jule_make_install_error(interp, cap_val, status, cap_sym);
                goto out;
            }
        }

        for (i = 0; i < n_params; i += 1) {
            arg_sym = params[i];
            JULE_ASSERT(arg_sym->type == JULE_SYMBOL);

            status = jule_eval(interp, values[i], &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out;
            }

            arg_val = jule_copy_force(ev);
            jule_free_value(ev);

            status = jule_install_common(interp, local_symtab, arg_sym->symbol_id, arg_val, 1);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                jule_make_install_error(interp, arg_val, status, arg_sym->symbol_id);
                goto out;
            }
        }

        jule_pushlocal_symtab(interp, local_symtab);

        expr   = jule_elem(fn->eval_values, 1 + lambda_params);
        status = jule_eval(interp, expr, &ev);
        if (status != JULE_SUCCESS) {
            jule_pop(interp->local_symtab_stack);
            jule_free_symtab(local_symtab);
            *result = NULL;
            goto out;
        }
        *result = ev;

        if ((*result)->local) {
            *result = jule_copy_force(*result);
        }

        status = jule_pop_local_symtab(interp, tree);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
    } else if (fn->type == JULE_LIST || fn->type == JULE_OBJECT) {
        builtin.type = _JULE_BUILTIN_FN;
        builtin.line = fn->line;
        builtin.col  = fn->col;

        if (fn->type == JULE_LIST) {
            builtin.builtin_fn = jule_builtin_elem;
        } else if (fn->type == JULE_OBJECT) {
            builtin.builtin_fn = jule_builtin_field;
        }

        container_bt_entry = JULE_MALLOC(sizeof(*container_bt_entry));

        container_bt_entry->file = interp->cur_file;
        container_bt_entry->fn   = &builtin;

        container_args    = alloca(sizeof(*container_args) * (n_values + 1));
        container_args[0] = fn;
        memcpy(container_args + 1, values, sizeof(*container_args) * n_values);

        interp->backtrace = jule_push(interp->backtrace, container_bt_entry);

        status = builtin.builtin_fn(interp, tree, n_values + 1, container_args, result);

        jule_pop(interp->backtrace);
        JULE_FREE(container_bt_entry);
    } else {
        status = JULE_ERR_BAD_INVOKE;
        jule_make_bad_invoke_error(interp, fn, fn->type);
        goto out;
    }

out:;

    interp->last_popped_builtin_fn = bt_entry->fn->type == _JULE_BUILTIN_FN
                                        ? bt_entry->fn->builtin_fn
                                        : NULL;

    jule_pop(interp->backtrace);
    JULE_FREE(bt_entry);

    interp->cur_file = save_file;
    return status;
}

static Jule_Status jule_eval(Jule_Interp *interp, Jule_Value *value, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *lookup;
    Jule_Value   *fn;
    Jule_Value  **arg_values;
    unsigned      n_args;

    status  = JULE_SUCCESS;
    *result = NULL;

    if (interp->eval_callback != NULL) {
        status = interp->eval_callback(value);
        if (status != JULE_SUCCESS) {
            jule_make_interp_error(interp, value, status);
            goto out;
        }
    }

    switch (value->type) {
        case JULE_NIL:
        case JULE_NUMBER:
        case JULE_STRING:
        case JULE_LIST:
        case JULE_OBJECT:
            *result = jule_copy(value);
            goto out;

        case JULE_SYMBOL:
            if ((lookup = jule_lookup(interp, value->symbol_id)) == NULL) {
                status = JULE_ERR_LOOKUP;
                jule_make_lookup_error(interp, value, value->symbol_id);
                *result = NULL;
                goto out;
            }
            switch (lookup->type) {
                case JULE_SYMBOL:
                case _JULE_TREE:
                case _JULE_TREE_LINE_LEADER:
                    status = jule_eval(interp, lookup, result);
                    goto out;
                case _JULE_FN:
                case _JULE_BUILTIN_FN:
                case _JULE_LAMBDA:
                    fn          = lookup;
                    arg_values  = NULL;
                    n_args      = 0;
                    goto invoke;
                default:
                    *result = jule_copy(lookup);
                    goto out;

            }
            break;

        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
            JULE_ASSERT(jule_len(value->eval_values) >= 1);

            fn = jule_elem(value->eval_values, 0);

            if (fn->type == JULE_SYMBOL) {
                if ((lookup = jule_lookup(interp, fn->symbol_id)) == NULL) {
                    status = JULE_ERR_LOOKUP;
                    jule_make_lookup_error(interp, value, fn->symbol_id);
                    *result = NULL;
                    goto out;
                }
                fn = lookup;
            } else {
                status = jule_eval(interp, fn, &fn);
                if (status != JULE_SUCCESS) {
                    *result = NULL;
                    goto out;
                }
            }

            switch (fn->type) {
                case _JULE_FN:
                case _JULE_BUILTIN_FN:
                case _JULE_LAMBDA:
                case JULE_LIST:
                case JULE_OBJECT:
                    arg_values = (Jule_Value**)value->eval_values->data + 1;
                    n_args     = jule_len(value->eval_values) - 1;
                    break;

                default:;
                    status = JULE_ERR_BAD_INVOKE;
                    jule_make_bad_invoke_error(interp, value, fn->type);
                    *result = NULL;
                    goto out;
                    break;
            }

invoke:;
            fn->line = value->line; /* @bad */
            fn->col  = value->col; /* @bad */

            status = jule_invoke(interp, value, fn, n_args, arg_values, result);
            if (status != JULE_SUCCESS) {
                jule_free_value(fn);
                *result = NULL;
                goto out;
            }
            jule_free_value(fn);
            break;

        default:
            JULE_ASSERT(0);
            break;
    }

out:;
    if (*result != NULL) {
        (*result)->line = value->line;
        (*result)->col  = value->col;
    }

    return status;
}



static Jule_Status jule_args(Jule_Interp *interp, Jule_Value *tree, const char *legend, unsigned n_values, Jule_Value **values, ...) {
    Jule_Status   status;
    va_list       args;
    unsigned      count;
    unsigned      i;
    int           no_eval;
    int           deep_copy;
    int           c;
    Jule_Value   *v;
    Jule_Value  **ve_ptr;
    va_list       cleanup_args;
    unsigned      j;
    Jule_Value   *cpy;
    int           t;

    status = JULE_SUCCESS;

    va_start(args, values);

    count = 0;
    for (i = 0; i < strlen(legend); i += 1) {
        count += legend[i] != '-' && legend[i] != '!';
    }

    i         = 0;
    no_eval   = 0;
    deep_copy = 0;
    while ((c = *legend)) {
        if (c == '-') {
            no_eval = 1;
            goto nextc;
        }
        if (c == '!') {
            deep_copy = 1;
            goto nextc;
        }

        if (i == n_values) {
            status = JULE_ERR_ARITY;
            jule_make_arity_error(interp, tree, count, n_values, 0);
            goto out;
        }

        v = values[i];

        ve_ptr = va_arg(args, Jule_Value**);

        if (no_eval) {
            if (deep_copy) {
                *ve_ptr = jule_copy_force(v);
            } else {
                *ve_ptr = jule_copy(v);
            }
        } else {
            status = jule_eval(interp, v, ve_ptr);
            if (status != JULE_SUCCESS) {
                va_start(cleanup_args, values);
                for (j = 0; j < i; j += 1) {
                    ve_ptr = va_arg(cleanup_args, Jule_Value**);
                    jule_free_value(*ve_ptr);
                    *ve_ptr = NULL;
                }
                va_end(cleanup_args);
                goto out;
            }
            if (deep_copy) {
                cpy = jule_copy_force(*ve_ptr);
                jule_free_value(*ve_ptr);
                *ve_ptr = cpy;
            }
        }

        (*ve_ptr)->line = v->line;
        (*ve_ptr)->col  = v->col;

        switch (c) {
            case '0': t = JULE_NIL;             break;
            case 'n': t = JULE_NUMBER;          break;
            case 's': t = JULE_STRING;          break;
            case '$': t = JULE_SYMBOL;          break;
            case 'l': t = JULE_LIST;            break;
            case 'o': t = JULE_OBJECT;          break;
            case '#': t = _JULE_LIST_OR_OBJECT; break;
            case 'k': t = _JULE_KEYLIKE;        break;
            case 'x': t = _JULE_TREE;           break;
            case '*': t = -1;                   break;
            default:  t = JULE_UNKNOWN;         break;
        }

        if (t == _JULE_LIST_OR_OBJECT
        &&  ((*ve_ptr)->type == JULE_LIST || (*ve_ptr)->type == JULE_OBJECT)) {

            /* Fine. */
        } else if (t == _JULE_KEYLIKE && JULE_TYPE_IS_KEYLIKE((*ve_ptr)->type)) {

            /* Fine. */
        } else if (t == _JULE_TREE && ((*ve_ptr)->type == _JULE_TREE || (*ve_ptr)->type == _JULE_TREE_LINE_LEADER)) {

            /* Fine. */
        } else if (t >= 0 && (*ve_ptr)->type != t) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, v, t, (*ve_ptr)->type);
            goto out;
        }

        i += 1;

        no_eval = deep_copy = 0;

nextc:;
        legend += 1;
    }

    if (i != n_values) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, count, n_values, 0);
        goto out;
    }

out:;
    va_end(args);

    return status;
}

static Jule_Status jule_builtin_eval(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *val;

    status = jule_args(interp, tree, "*", n_values, values, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, val, result);

    jule_free_value(val);

out:;
    return status;
}

static Jule_Status jule_builtin_set(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "-$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    status = jule_install_var(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_local(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "-$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    status = jule_install_local(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_ref(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;

    status = jule_args(interp, tree, "-$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    JULE_ASSERT(val->type != _JULE_REF && "this should not happen");

    if (!val->in_symtab) {
        *result = NULL;
        jule_make_install_error(interp, tree, JULE_ERR_REF_OF_TRANSIENT, sym->symbol_id);
        goto out_free;
    }

    val = jule_ref_value(val);

    status = jule_install_local(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_eset(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    status = jule_install_var(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_elocal(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    status = jule_install_local(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_eref(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;

    status = jule_args(interp, tree, "$*", n_values, values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    JULE_ASSERT(val->type != _JULE_REF && "this should not happen");

    if (!val->in_symtab) {
        *result = NULL;
        jule_make_install_error(interp, tree, JULE_ERR_REF_OF_TRANSIENT, sym->symbol_id);
        goto out_free;
    }

    val = jule_ref_value(val);

    status = jule_install_local(interp, sym->symbol_id, val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, tree, status, sym->symbol_id);
        jule_free_value(val);
        goto out_free;
    }

    *result = val;

out_free:;
    jule_free_value(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_fn(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *def_tree;
    Jule_Value  *it;
    Jule_Value  *sym;
    Jule_Value  *fn;

    status = JULE_SUCCESS;

    if (n_values < 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 1);
        *result = NULL;
        goto out;
    }

    def_tree = values[0];

    if (def_tree->type == _JULE_TREE || def_tree->type == _JULE_TREE_LINE_LEADER) {
        FOR_EACH(def_tree->eval_values, it) {
            if (it->type != JULE_SYMBOL) {
                status = JULE_ERR_TYPE;
                jule_make_type_error(interp, def_tree, JULE_SYMBOL, it->type);
                *result = NULL;
                goto out;
            }
        }
        sym = jule_elem(def_tree->eval_values, 0);
    } else if (def_tree->type == JULE_SYMBOL) {
        sym = def_tree;
    } else {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, def_tree, JULE_SYMBOL, def_tree->type);
        *result = NULL;
        goto out;
    }

    fn       = jule_copy(tree);
    fn->type = _JULE_FN;

    status = jule_install_var(interp, sym->symbol_id, fn);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, sym, status, sym->symbol_id);
        jule_free_value(fn);
        goto out;
    }

    *result = fn;

out:;
    return status;
}

static Jule_Status jule_builtin_localfn(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *def_tree;
    Jule_Value  *it;
    Jule_Value  *sym;
    Jule_Value  *fn;

    status = JULE_SUCCESS;

    if (n_values < 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 1);
        *result = NULL;
        goto out;
    }

    def_tree = values[0];

    if (def_tree->type == _JULE_TREE) {
        FOR_EACH(def_tree->eval_values, it) {
            if (it->type != JULE_SYMBOL) {
                status = JULE_ERR_TYPE;
                jule_make_type_error(interp, def_tree, JULE_SYMBOL, it->type);
                *result = NULL;
                goto out;
            }
        }
        sym = jule_elem(def_tree->eval_values, 0);
    } else if (def_tree->type == JULE_SYMBOL) {
        sym = def_tree;
    } else {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, def_tree, JULE_SYMBOL, def_tree->type);
        *result = NULL;
        goto out;
    }

    fn       = jule_copy(tree);
    fn->type = _JULE_FN;

    status = jule_install_local(interp, sym->symbol_id, fn);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_install_error(interp, sym, status, sym->symbol_id);
        jule_free_value(fn);
        goto out;
    }

    *result = fn;

out:;
    return status;
}

static void _jule_collect_lambda_free_variables(Jule_Interp *interp, Jule_Value *tree, Jule_Array *bounds, Jule_Array **frees) {
    Jule_Value *it;
    Jule_Value *first;

    switch (tree->type) {
        case JULE_SYMBOL:
            FOR_EACH(bounds, it) {
                if (tree->symbol_id == it->symbol_id) { return; }
            }
            FOR_EACH(*frees, it) {
                if (tree->symbol_id == it->symbol_id) { return; }
            }

            *frees = jule_push(*frees, tree);
            break;

        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
            first = jule_elem(tree->eval_values, 0);

            if (first->type == JULE_SYMBOL
            &&  ( first->symbol_id == jule_get_string_id(interp, "lambda")
               || first->symbol_id == jule_get_string_id(interp, "fn")
               || first->symbol_id == jule_get_string_id(interp, "localfn")
               || first->symbol_id == jule_get_string_id(interp, "quote"))) {

                /* Skip these forms. */
                return;
            } else {
                FOR_EACH(tree->eval_values, it) {
                    _jule_collect_lambda_free_variables(interp, it, bounds, frees);
                }
            }
            break;
    }
}

static Jule_Status jule_builtin_lambda(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *def_tree;
    Jule_Array        *bounds = JULE_ARRAY_INIT;
    Jule_Value        *it;
    Jule_Value        *fn;
    Jule_Closure_Info *closure;
    Jule_Array        *frees = JULE_ARRAY_INIT;
    Jule_Value        *lookup;

    status = JULE_SUCCESS;

    if (n_values < 1 || n_values > 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2 - (n_values == 1), n_values, 1 - (n_values == 2));
        *result = NULL;
        goto out;
    }

    if (n_values == 2) {
        def_tree = values[0];

        if (def_tree->type == _JULE_TREE) {
            FOR_EACH(def_tree->eval_values, it) {
                if (it->type != JULE_SYMBOL) {
                    status = JULE_ERR_TYPE;
                    jule_make_type_error(interp, def_tree, JULE_SYMBOL, it->type);
                    *result = NULL;
                    goto out;
                }
                bounds = jule_push(bounds, it);
            }
        } else {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, def_tree, _JULE_TREE, def_tree->type);
            *result = NULL;
            goto out;
        }
    }

    fn       = jule_copy(tree);
    fn->type = _JULE_LAMBDA;

    closure = JULE_MALLOC(sizeof(*closure));

    closure->cur_file = fn->eval_values->aux;
    closure->captures = hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash);

    _jule_collect_lambda_free_variables(interp, values[n_values == 2], bounds, &frees);

    FOR_EACH(frees, it) {
        lookup = jule_lookup(interp, it->symbol_id);
        if (lookup != NULL) {
            hash_table_insert(closure->captures, it->symbol_id, jule_copy_force(lookup));
        }
    }

    jule_free_array(frees);
    jule_free_array(bounds);

    fn->eval_values = jule_array_set_aux(fn->eval_values, closure);

    *result = fn;

out:;
    return status;
}

static Jule_Status jule_builtin_id(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *value;
    Jule_Value  *ev;
    Jule_Value  *lookup;

    status = jule_args(interp, tree, "-*", n_values, values, &value);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    switch (value->type) {
        case JULE_NUMBER:
        case JULE_STRING:
        case JULE_LIST:
        case JULE_OBJECT:
        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
            status = jule_eval(interp, value, &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out_free;
            }
            break;
        case JULE_SYMBOL:
            lookup = jule_lookup(interp, value->symbol_id);
            if (lookup == NULL) {
                status = JULE_ERR_LOOKUP;
                jule_make_lookup_error(interp, value, value->symbol_id);
                *result = NULL;
                goto out_free;
            }

            ev = jule_copy(lookup);
            break;
        default:
            JULE_ASSERT(0);
            break;
    }

    *result = ev;

out_free:;
    jule_free_value(value);

out:;
    return status;
}

static Jule_Status jule_builtin_quote(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *value;

    status = jule_args(interp, tree, "-*", n_values, values, &value);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_copy(value);

    jule_free_value(value);

out:;
    return status;
}


static Jule_Status jule_builtin_add(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number + b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_sub(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number - b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_mul(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number * b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_div(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (b->number == 0) {
        *result = jule_number_value(0);
    } else {
        *result = jule_number_value(a->number / b->number);
    }

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_idiv(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (b->number == 0) {
        *result = jule_number_value(0);
    } else {
        *result = jule_number_value((long long)a->number / (long long)b->number);
    }

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_mod(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if ((long long)b->number == 0) {
        *result = jule_number_value(0);
    } else {
        *result = jule_number_value((long long)a->number % (long long)b->number);
    }

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_inc(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number);

    a->number += 1;

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status jule_builtin_dec(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number);

    a->number -= 1;

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status jule_builtin_equ(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "**", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(jule_equal(a, b));

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_neq(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "**", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(!jule_equal(a, b));

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_lss(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number < b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_leq(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number <= b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_gtr(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number > b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_geq(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", n_values, values, &a, &b);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number >= b->number);

    jule_free_value(a);
    jule_free_value(b);

out:;
    return status;
}

static Jule_Status jule_builtin_not(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;

    status = jule_args(interp, tree, "n", n_values, values, &a);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    *result = jule_number_value(a->number == 0);

    jule_free_value(a);

out:;
    return status;
}

static Jule_Status jule_builtin_and(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    int          short_circuit;
    unsigned     i;
    Jule_Value  *cond;
    Jule_Value  *ev;

    status = JULE_SUCCESS;

    if (n_values < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 1, n_values, 1);
        *result = NULL;
        goto out;
    }

    short_circuit = 0;

    for (i = 0; i < n_values; i += 1) {
        cond   = values[i];
        status = jule_eval(interp, cond, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (ev->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, ev, JULE_NUMBER, ev->type);
            jule_free_value(ev);
            *result = NULL;
            goto out;
        }

        short_circuit = ev->number == 0;

        jule_free_value(ev);

        if (short_circuit) {
            *result = jule_number_value(0);
            goto out;
        }
    }

    *result = jule_number_value(1);

out:;
    return status;
}

static Jule_Status jule_builtin_or(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    int          short_circuit;
    unsigned     i;
    Jule_Value  *cond;
    Jule_Value  *ev;

    status = JULE_SUCCESS;

    if (n_values < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 1, n_values, 1);
        *result = NULL;
        goto out;
    }

    short_circuit = 0;

    for (i = 0; i < n_values; i += 1) {
        cond = values[i];
        status = jule_eval(interp, cond, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (ev->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, ev, JULE_NUMBER, ev->type);
            jule_free_value(ev);
            *result = NULL;
            goto out;
        }

        short_circuit = ev->number != 0;

        jule_free_value(ev);

        if (short_circuit) {
            *result = jule_number_value(1);
            goto out;
        }
    }

    *result = jule_number_value(0);

out:;
    return status;
}

static Jule_Status jule_builtin_string(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *val;
    char        *s;

    status = jule_args(interp, tree, "*", n_values, values, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    s = jule_to_string(interp, val, JULE_NO_QUOTE);
    *result = jule_string_value(interp, s);
    JULE_FREE(s);

    jule_free_value(val);

out:;
    return status;
}

static Jule_Status jule_builtin_symbol(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *str;
    const Jule_String *string;

    status = jule_args(interp, tree, "s", n_values, values, &str);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    string  = jule_get_string(interp, str->string_id);
    *result = jule_symbol_value(interp, string->chars);

    jule_free_value(str);

out:;
    return status;
}

static Jule_Status jule_builtin_pad(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *w;
    Jule_Value  *val;
    int          width;
    int          ljust;
    char        *s;
    int          len;
    int          padding;
    char        *padded;

    status = jule_args(interp, tree, "n*", n_values, values, &w, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    width = (int)w->number;
    ljust = width < 0;
    s     = jule_to_string(interp, val, JULE_NO_QUOTE);
    len   = strlen(s);

    if (ljust) { width = -width; }

    padding = width > len
                ? width - len
                : 0;

    padded = JULE_MALLOC(len + padding + 1);
    memset(padded, ' ', len + padding);
    memcpy(padded + ((!ljust) * padding), s, len);
    padded[len + padding] = 0;

    *result = jule_string_value(interp, padded);

    JULE_FREE(padded);
    JULE_FREE(s);

    jule_free_value(w);
    jule_free_value(val);

out:;
    return status;
}

static Jule_Status _jule_builtin_print(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result, int nl) {
    Jule_Status  status;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;

    (void)tree;

    status = JULE_SUCCESS;

    for (i = 0; i < n_values; i += 1) {
        it = values[i];
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
        jule_print(interp, ev, 0);

        if (i == n_values - 1) {
            *result = ev;
        } else {
            jule_free_value(ev);
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

    if (nl) { jule_output(interp, "\n", 1); }

out:;
    return status;
}

static Jule_Status jule_builtin_print(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    return _jule_builtin_print(interp, tree, n_values, values, result, 0);
}

static Jule_Status jule_builtin_println(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    return _jule_builtin_print(interp, tree, n_values, values, result, 1);
}

static Jule_Status jule_builtin_fmt(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *fmt;
    const Jule_String *fstring;
    unsigned           n;
    unsigned           extra;
    char               last;
    char               c;
    unsigned           i;
    Jule_Array        *strings = JULE_ARRAY_INIT;
    Jule_Value        *it;
    Jule_Value        *ev;
    char              *s;
    int                len;
    char              *formatted;
    char              *ins;
    int                sublen;

    status = JULE_SUCCESS;

    if (n_values < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 1, n_values, 1);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[0], &fmt);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (fmt->type != JULE_STRING) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, fmt, JULE_STRING, fmt->type);
        *result = NULL;
        goto out_free_fmt;
    }

    fstring = jule_get_string(interp, fmt->string_id);
    n       = 0;
    extra   = 0;
    last    = 0;
    for (i = 0; i < fstring->len; i += 1) {
        c = fstring->chars[i];
        if (c == '%') {
            extra += 1;
            if (last != '\\') {
                n += 1;
            }
        }
        last = c;
    }

    if (n_values - 1 != n) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, n + 1, n_values, 0);
        *result = NULL;
        goto out_free_fmt;
    }

    for (i = 1; i < n_values; i += 1) {
        it = values[i];
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free_strings;
        }
        s = jule_to_string(interp, ev, JULE_NO_QUOTE);
        strings = jule_push(strings, s);
        jule_free_value(ev);
    }

    len = fstring->len - extra;
    FOR_EACH(strings, s) {
        len += strlen(s);
    }

    formatted = JULE_MALLOC(len + 1);
    ins       = formatted;

    n    = 0;
    last = 0;
    for (i = 0; i < fstring->len; i += 1) {
        c = fstring->chars[i];
        if (c == '\\' && i < fstring->len - 1 && fstring->chars[i + 1] == '%') {
            /* skip */
        } else if (c == '%' && last != '\\') {
            s      = jule_elem(strings, n);
            sublen = strlen(s);
            memcpy(ins, s, sublen);
            ins += sublen;
            n += 1;
        } else {
            *ins  = c;
            ins  += 1;
        }
        last = c;
    }

    formatted[len] = 0;

    *result = jule_string_value(interp, formatted);

    JULE_FREE(formatted);

out_free_strings:;
    FOR_EACH(strings, s) {
        JULE_FREE(s);
    }
    jule_free_array(strings);

out_free_fmt:;
    jule_free_value(fmt);

out:;
    return status;
}

static Jule_Status jule_builtin_num_fmt(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *fmt;
    Jule_Value  *val;
    char         fbuff[128];
    char         buff[128];

    status = jule_args(interp, tree, "sn", n_values, values, &fmt, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    snprintf(fbuff, sizeof(fbuff), "%%%s", jule_get_string(interp, fmt->string_id)->chars);
    snprintf(buff, sizeof(buff), fbuff, val->number);

    *result = jule_string_value(interp, buff);

    jule_free_value(fmt);
    jule_free_value(val);

out:;
    return status;
}


static Jule_Status jule_builtin_parse_int(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *s;
    long long    i;

    status = jule_args(interp, tree, "s", n_values, values, &s);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (sscanf(jule_get_string(interp, s->string_id)->chars, "%lld", &i) == 1) {
        *result = jule_number_value(i);
    } else {
        *result = jule_nil_value();
    }

    jule_free_value(s);

out:;
    return status;
}

static Jule_Status jule_builtin_parse_hex(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status         status;
    Jule_Value         *s;
    unsigned long long  i;

    status = jule_args(interp, tree, "s", n_values, values, &s);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (sscanf(jule_get_string(interp, s->string_id)->chars, "%llx", &i) == 1) {
        *result = jule_number_value(i);
    } else {
        *result = jule_nil_value();
    }

    jule_free_value(s);

out:;
    return status;
}

static Jule_Status jule_builtin_parse_float(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *s;
    double       d;

    status = jule_args(interp, tree, "s", n_values, values, &s);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (sscanf(jule_get_string(interp, s->string_id)->chars, "%lg", &d) == 1) {
        *result = jule_number_value(d);
    } else {
        *result = jule_nil_value();
    }

    jule_free_value(s);

out:;
    return status;
}

static Jule_Status jule_builtin_select(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;
    Jule_Value  *then;

    status = JULE_SUCCESS;

    if (n_values != 3) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 3, n_values, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[0], &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    then   = values[1 + (cond->number == 0)];
    status = jule_eval(interp, then, &then);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free_cond;
    }

    *result = then;

out_free_cond:;
    jule_free_value(cond);

out:;
    return status;
}

static Jule_Status jule_builtin_do(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;

    status  = JULE_SUCCESS;
    *result = NULL;

    if (n_values < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 1, n_values, 1);
        *result = NULL;
        goto out;
    }

    for (i = 0; i < n_values; i += 1) {
        it     = values[i];
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            if (*result != NULL) {
                jule_free_value(*result);
            }
            *result = NULL;
            goto out;
        }

        if (i == n_values - 1) {
            *result = ev;
        } else {
            jule_free_value(ev);
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out:;
    return status;
}

static Jule_Status jule_builtin_if(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;
    unsigned     truth;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;

    status = JULE_SUCCESS;

    if (n_values < 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 2);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[0], &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    truth = !!(long long)cond->number;

    if (truth) {
        for (i = 1; i < n_values; i += 1) {
            it     = values[i];
            status = jule_eval(interp, it, &ev);
            if (status != JULE_SUCCESS) {
                if (*result != NULL) {
                    jule_free_value(*result);
                }
                *result = NULL;
                goto out_free_cond;
            }

            if (i == n_values - 1) {
                *result = ev;
            } else {
                jule_free_value(ev);
            }
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

    interp->last_if_was_true = truth;

out_free_cond:;
    jule_free_value(cond);

out:;
    return status;
}

static Jule_Status jule_builtin_elif(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;
    unsigned     truth;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;

    status = JULE_SUCCESS;

    if (interp->last_popped_builtin_fn != jule_builtin_if
    &&  interp->last_popped_builtin_fn != jule_builtin_elif) {
        status  = JULE_ERR_MUST_FOLLOW_IF;
        *result = NULL;
        jule_make_must_follow_if_error(interp, tree);
        goto out;
    }

    if (n_values < 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 2);
        *result = NULL;
        goto out;
    }

    if (!interp->last_if_was_true) {
        status = jule_eval(interp, values[0], &cond);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (cond->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, cond, JULE_NUMBER, cond->type);
            goto out_free_cond;
        }

        truth = !!(long long)cond->number;

        if (truth) {
            for (i = 1; i < n_values; i += 1) {
                it     = values[i];
                status = jule_eval(interp, it, &ev);
                if (status != JULE_SUCCESS) {
                    if (*result != NULL) {
                        jule_free_value(*result);
                    }
                    *result = NULL;
                    goto out_free_cond;
                }

                if (i == n_values - 1) {
                    *result = ev;
                } else {
                    jule_free_value(ev);
                }
            }
        }

        interp->last_if_was_true = truth;

out_free_cond:;
        jule_free_value(cond);
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out:;
    return status;
}

static Jule_Status jule_builtin_else(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;

    status  = JULE_SUCCESS;
    *result = NULL;

    if (interp->last_popped_builtin_fn != jule_builtin_if
    &&  interp->last_popped_builtin_fn != jule_builtin_elif) {
        status  = JULE_ERR_MUST_FOLLOW_IF;
        *result = NULL;
        jule_make_must_follow_if_error(interp, tree);
        goto out;
    }

    if (n_values < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 1, n_values, 1);
        *result = NULL;
        goto out;
    }

    if (!interp->last_if_was_true) {
        for (i = 0; i < n_values; i += 1) {
            it     = values[i];
            status = jule_eval(interp, it, &ev);
            if (status != JULE_SUCCESS) {
                if (*result != NULL) {
                    jule_free_value(*result);
                }
                *result = NULL;
                goto out;
            }

            if (i == n_values - 1) {
                *result = ev;
            } else {
                jule_free_value(ev);
            }
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out:;
    return status;
}


static Jule_Status jule_builtin_while(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *_cond;
    Jule_Value  *expr;
    Jule_Value  *cond;
    int          cont;
    unsigned     i;
    Jule_Value  *_expr;
    Jule_Value  *expr_cpy;

    status = JULE_SUCCESS;

    *result = NULL;

    if (n_values < 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 1);
        *result = NULL;
        goto out;
    }

    _cond = values[0];
    expr  = NULL;

    for (;;) {
        status = jule_eval(interp, _cond, &cond);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (cond->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, cond, JULE_NUMBER, cond->type);
            jule_free_value(cond);
            goto out;
        }

        cont = cond->number != 0;

        jule_free_value(cond);

        if (!cont) {
            *result = expr != NULL
                        ? expr
                        : jule_nil_value();
            break;
        }

        if (expr != NULL) {
            jule_free_value(expr);
        }

        for (i = 1; i < n_values; i += 1) {
            _expr  = values[i];
            status = jule_eval(interp, _expr, &expr);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out;
            }

            if (i < n_values - 1) {
                jule_free_value(expr);
                expr = NULL;
            }
        }

        /* Get a copy of the resulting value that we know can't be deleted while running the condition expression. */
        expr_cpy = jule_copy_force(expr);
        jule_free_value(expr);
        expr = expr_cpy;
    }

out:;
    return status;
}

static Jule_Status jule_builtin_foreach(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *sym;
    Jule_Value   *_container;
    Jule_Value   *container;
    Jule_Value   *expr;
    unsigned      i;
    unsigned      j;
    Jule_Value   *it;
    Jule_Value   *ev;
    Jule_Value  **val;

    *result = NULL;

    if (n_values < 3) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 3, n_values, 1);
        *result = NULL;
        goto out;
    }

    sym        = values[0];
    _container = values[1];

    status = jule_eval(interp, _container, &container);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (container->type != JULE_LIST
    &&  container->type != JULE_OBJECT) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, container, _JULE_LIST_OR_OBJECT, container->type);
        goto out_free;
    }

    JULE_BORROW(container);
    interp->iter_vals = jule_push(interp->iter_vals, container);

    if (container->type == JULE_LIST) {
        i = 0;
        FOR_EACH(container->list, it) {
            JULE_BORROWER(it);
            status = jule_install_local(interp, sym->symbol_id, it);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                jule_make_install_error(interp, sym, status, sym->symbol_id);
                goto out_unborrow;
            }

            for (j = 2; j < n_values; j += 1) {
                expr   = values[j];
                status = jule_eval(interp, expr, &ev);
                if (status != JULE_SUCCESS) {
                JULE_UNBORROWER(it);
                jule_uninstall_local_no_free(interp, sym->symbol_id);
                    *result = NULL;
                    goto out_unborrow;
                }

                if (j < n_values - 1) {
                    jule_free_value(ev);
                    ev = NULL;
                }
            }

            i += 1;

            if (i == jule_len(container->list)) {
                if (ev == it) {
                    ev = jule_copy_force(it);
                }
                *result = ev;
            } else {
                jule_free_value(ev);
            }

            JULE_UNBORROWER(it);
            if (jule_lookup_local_only(interp, sym->symbol_id) == it) {
                status = jule_uninstall_local_no_free(interp, sym->symbol_id);
                if (status != JULE_SUCCESS) {
                    *result = NULL;
                    jule_make_install_error(interp, sym, status, sym->symbol_id);
                    goto out_unborrow;
                }
            }
        }

    } else {
        i = 0;
        hash_table_traverse((_Jule_Object)container->object, it, val) {
            it = *val;

            JULE_BORROWER(it);
            status = jule_install_local(interp, sym->symbol_id, it);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                jule_make_install_error(interp, sym, status, sym->symbol_id);
                goto out_unborrow;
            }

            for (j = 2; j < n_values; j += 1) {
                expr   = values[j];
                status = jule_eval(interp, expr, &ev);
                if (status != JULE_SUCCESS) {
                JULE_UNBORROWER(it);
                jule_uninstall_local_no_free(interp, sym->symbol_id);
                    *result = NULL;
                    goto out_unborrow;
                }

                if (j < n_values - 1) {
                    jule_free_value(ev);
                    ev = NULL;
                }
            }

            i += 1;

            if (i == hash_table_len((_Jule_Object)container->object)) {
                if (ev == it) {
                    ev = jule_copy_force(it);
                }
                *result = ev;
            } else {
                jule_free_value(ev);
            }

            JULE_UNBORROWER(it);
            if (jule_lookup_local_only(interp, sym->symbol_id) == it) {
                status = jule_uninstall_local_no_free(interp, sym->symbol_id);
                if (status != JULE_SUCCESS) {
                    *result = NULL;
                    jule_make_install_error(interp, sym, status, sym->symbol_id);
                    goto out_unborrow;
                }
            }
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out_unborrow:;
    jule_pop(interp->iter_vals);
    JULE_UNBORROW(container);

out_free:;
    jule_free_value(container);

out:;
    return status;
}

static Jule_Status jule_builtin_list(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;
    Jule_Value  *tmp;

    (void)tree;

    status = JULE_SUCCESS;

    list = jule_list_value();

    for (i = 0; i < n_values; i += 1) {
        it     = values[i];
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free;
        }

        if (ev->in_symtab) {
            tmp = jule_copy_force(ev);
            jule_free_value(ev);
            ev = tmp;
        }

        list->list = jule_push(list->list, ev);
    }

    *result = list;
    goto out;

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_range(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *beg;
    Jule_Value  *end;
    Jule_Value  *list;
    long long    i;

    (void)tree;

    status = JULE_SUCCESS;

    status = jule_args(interp, tree, "nn", n_values, values, &beg, &end);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    list = jule_list_value();

    if ((long long)beg->number <= (long long)end->number) {
        for (i = (long long)beg->number; i < (long long)end->number; i += 1) {
            list->list = jule_push(list->list, jule_number_value(i));
        }
    } else {
        for (i = (long long)beg->number; i > (long long)end->number; i -= 1) {
            list->list = jule_push(list->list, jule_number_value(i));
        }
    }

    *result = list;

    jule_free_value(beg);
    jule_free_value(end);

out:;
    return status;
}

static Jule_Status jule_builtin_dot(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *first;
    Jule_Value  *second;
    Jule_Value  *tmp;
    Jule_Value  *list;

    status = jule_args(interp, tree, "**", n_values, values, &first, &second);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (first->in_symtab) {
        tmp = jule_copy_force(first);
        jule_free_value(first);
        first = tmp;
    }
    if (second->in_symtab) {
        tmp = jule_copy_force(second);
        jule_free_value(second);
        second = tmp;
    }

    list = jule_list_value();

    list->list = jule_push(list->list, first);
    list->list = jule_push(list->list, second);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_object(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *object;
    unsigned     i;
    Jule_Value  *it;
    Jule_Value  *ev;
    Jule_Value  *key;
    Jule_Value  *val;

    (void)tree;

    status = JULE_SUCCESS;

    object = jule_object_value();

    for (i = 0; i < n_values; i += 1) {
        it     = values[i];
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free_object;
        }

        if (ev->type != JULE_LIST) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, ev, JULE_LIST, ev->type);
            *result = NULL;
            goto out_free_list;
        }

        if (jule_len(ev->list) < 2) {
            status = JULE_ERR_MISSING_VAL;
            jule_make_interp_error(interp, it, status);
            *result = NULL;
            goto out_free_list;
        }

        key = jule_copy_force(jule_elem(ev->list, 0));
        val = jule_copy_force(jule_elem(ev->list, 1));

        if (key->type != JULE_STRING && key->type != JULE_NUMBER) {
            status = JULE_ERR_OBJECT_KEY_TYPE;
            jule_make_object_key_type_error(interp, key, key->type);
            *result = NULL;
            goto out_free_list;
        }

        jule_free_value(ev);

        jule_insert(object, key, val);
    }

    *result = object;
    goto out;

out_free_list:;
    jule_free_value(ev);

out_free_object:;
    jule_free_value(object);

out:;
    return status;
}

static Jule_Status jule_builtin_in(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    int           found;
    Jule_Value   *container;
    Jule_Value   *key;
    Jule_Value  **lookup;
    Jule_Value   *it;

    status = JULE_SUCCESS;

    found = 0;

    if (n_values != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[0], &container);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[1], &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free_container;
    }

    if (container->type == JULE_OBJECT) {
        if (key->type != JULE_NUMBER && key->type != JULE_STRING) {
            status = JULE_ERR_OBJECT_KEY_TYPE;
            jule_make_object_key_type_error(interp, key, key->type);
            *result = NULL;
            goto out_free_key;
        }

        lookup = hash_table_get_val((_Jule_Object)container->object, key);

        found = lookup != NULL;

    } else if (container->type == JULE_LIST) {
        FOR_EACH(container->list, it) {
            if (jule_equal(key, it)) {
                found = 1;
                break;
            }
        }
    } else {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, container, _JULE_LIST_OR_OBJECT, container->type);
        *result = NULL;
        goto out_free_key;
    }

    *result = jule_number_value(found);

out_free_key:;
    jule_free_value(key);

out_free_container:;
    jule_free_value(container);

out:;
    return status;
}


static Jule_Status jule_builtin_elem(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *idx;
    unsigned      i;
    Jule_Value   *val;

    status = jule_args(interp, tree, "ln", n_values, values, &list, &idx);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    i = (int)idx->number;

    if (i >= jule_len(list->list)) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, idx, jule_copy(idx));
        *result = NULL;
        goto out_free;
    }

    val            = jule_elem(list->list, i);
    val->in_symtab = list->in_symtab;
    val->local     = list->local;

    *result = jule_copy(val);

out_free:;
    jule_free_value(idx);
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_index(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *val;
    unsigned      i;
    Jule_Value   *it;

    status = jule_args(interp, tree, "l*", n_values, values, &list, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    i = 0;
    FOR_EACH(list->list, it) {
        if (jule_equal(val, it)) {
            *result = jule_number_value(i);
            break;
        }
        i += 1;
    }

    if (*result == NULL) {
        *result = jule_number_value(-1);
    }

    jule_free_value(val);
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_append(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *val;
    Jule_Value   *it;

    status = jule_args(interp, tree, "l!*", n_values, values, &list, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == list) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(list);
            jule_free_value(val);
            goto out;
        }
    }

    list->list = jule_push(list->list, val);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_pop(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    Jule_Value  *it;
    Jule_Value  *last;

    status = jule_args(interp, tree, "l", n_values, values, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == list) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(list);
            goto out;
        }
    }

    if (jule_len(list->list) <= 0) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, tree, jule_number_value(-1));
        *result = NULL;
        goto out_free;
    }

    last = jule_top(list->list);

    if (last->borrow_count) {
        jule_make_install_error(interp, tree, JULE_ERR_RELEASE_WHILE_BORROWED, NULL);
        *result = NULL;
        goto out_free;
    }

    *result = jule_pop(list->list);
    (*result)->in_symtab      = 0;
    (*result)->local          = 0;
    (*result)->borrow_count   = 0;
    (*result)->borrower_count = 0;

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_field(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *object;
    Jule_Value  *key;
    Jule_Value  *field;

    status = JULE_SUCCESS;

    if (n_values != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree, 2, n_values, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, values[0], &object);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (object->type != JULE_OBJECT) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, object, JULE_OBJECT, object->type);
        *result = NULL;
        goto out_free_object;
    }

    status = jule_eval(interp, values[1], &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free_object;
    }

    if (key->type != JULE_NUMBER && key->type != JULE_STRING) {
        status = JULE_ERR_OBJECT_KEY_TYPE;
        jule_make_object_key_type_error(interp, key, key->type);
        *result = NULL;
        goto out_free_key;
    }

    field = jule_field(object, key);

    if (field == NULL) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, key, jule_copy(key));
        *result = NULL;
        goto out_free_key;
    } else {
        field->in_symtab = object->in_symtab;
        field->local     = object->local;
        *result = jule_copy(field);
    }

out_free_key:;
    jule_free_value(key);

out_free_object:;
    jule_free_value(object);

out:;
    return status;
}

static Jule_Status jule_builtin_insert(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *object;
    Jule_Value  *key;
    Jule_Value  *val;
    Jule_Value  *it;

    status = jule_args(interp, tree, "o!k!*", n_values, values, &object, &key, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == object) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(object);
            jule_free_value(key);
            jule_free_value(val);
            goto out;
        }
    }

    if (jule_insert(object, key, val) == JULE_ERR_RELEASE_WHILE_BORROWED) {
        jule_make_install_error(interp, tree, JULE_ERR_RELEASE_WHILE_BORROWED, NULL);
        *result = NULL;
        jule_free_value(object);
        jule_free_value(key);
        jule_free_value(val);
        goto out;
    }

    *result = object;

out:;
    return status;
}

static Jule_Status jule_builtin_delete(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *object;
    Jule_Value  *key;
    Jule_Value  *it;

    status = jule_args(interp, tree, "ok", n_values, values, &object, &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == object) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(object);
            jule_free_value(key);
            goto out;
        }
    }

    if (jule_delete(object, key) == JULE_ERR_RELEASE_WHILE_BORROWED) {
        jule_make_install_error(interp, tree, JULE_ERR_RELEASE_WHILE_BORROWED, NULL);
        *result = NULL;
        jule_free_value(object);
        jule_free_value(key);
        goto out;
    }

    jule_free_value(key);

    *result = object;

out:;
    return status;
}

static Jule_Status jule_builtin_update_object(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *o1;
    Jule_Value   *o2;
    Jule_Value   *key;
    Jule_Value  **val;
    Jule_Value   *it;
    Jule_Value   *kcpy;
    Jule_Value   *vcpy;

    status = jule_args(interp, tree, "oo", n_values, values, &o1, &o2);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == o1) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(o1);
            jule_free_value(o2);
            goto out;
        }
    }

    hash_table_traverse((_Jule_Object)o2->object, key, val) {
        kcpy = jule_copy_force(key);
        vcpy = jule_copy_force(*val);
        if (jule_insert(o1, kcpy, vcpy) == JULE_ERR_RELEASE_WHILE_BORROWED) {
            jule_free_value_force(kcpy);
            jule_free_value_force(vcpy);
            jule_make_install_error(interp, tree, JULE_ERR_RELEASE_WHILE_BORROWED, NULL);
            *result = NULL;
            jule_free_value(o1);
            jule_free_value(o2);
            goto out;
        }
    }

    jule_free_value(o2);

    *result = o1;

out:;
    return status;
}

static Jule_Status jule_builtin_erase(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *idx;
    Jule_Value   *it;
    unsigned      i;
    Jule_Value   *val;

    status = jule_args(interp, tree, "ln", n_values, values, &list, &idx);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    FOR_EACH(interp->iter_vals, it) {
        if (it == list) {
            status  = JULE_ERR_MODIFY_WHILE_ITER;
            *result = NULL;
            jule_make_install_error(interp, tree, status, values[0]->type == JULE_SYMBOL ? values[0]->symbol_id : NULL);
            jule_free_value(list);
            jule_free_value(idx);
            goto out;
        }
    }

    i = (int)idx->number;

    jule_free_value(idx);

    if (i >= jule_len(list->list)) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, idx, jule_copy(idx));
        *result = NULL;
        jule_free_value(list);
        goto out;
    }

    val = jule_elem(list->list, i);
    if (val->borrow_count) {
        jule_make_install_error(interp, tree, JULE_ERR_RELEASE_WHILE_BORROWED, NULL);
        *result = NULL;
        jule_free_value(list);
        goto out;
    }

    jule_erase(list->list, (unsigned)idx->number);
    jule_free_value_force(val);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_keys(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *list;
    Jule_Value   *key;
    Jule_Value  **val;

    status = jule_args(interp, tree, "o", n_values, values, &object);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    list = jule_list_value();
    hash_table_traverse((_Jule_Object)object->object, key, val) {
        (void)val;
        list->list = jule_push(list->list, jule_copy_force(key));
    }

    jule_free_value(object);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_values(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *list;
    Jule_Value   *key;
    Jule_Value  **val;

    status = jule_args(interp, tree, "o", n_values, values, &object);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    list = jule_list_value();
    hash_table_traverse((_Jule_Object)object->object, key, val) {
        (void)key;
        list->list = jule_push(list->list, jule_copy_force(*val));
    }

    jule_free_value(object);

    *result = list;

out:;
    return status;
}

typedef struct {
    Jule_Interp *interp;
    Jule_Type    sort_type;
} _Jule_Sort_Arg;

static int jule_sort_value_cmp(const void *_a, const void *_b, void *_arg) {
    int               r;
    const Jule_Value *a;
    const Jule_Value *b;
    _Jule_Sort_Arg   *arg;
    double            ad;
    double            bd;
    char             *ac;
    char             *bc;
    const char       *as;
    const char       *bs;

    r   = 0;
    a   = *(const Jule_Value**)_a;
    b   = *(const Jule_Value**)_b;
    arg = _arg;

    if (a->type == JULE_NIL) { return -1; }
    if (b->type == JULE_NIL) { return  1; }

    if (arg->sort_type == JULE_NUMBER) {
        ad = a->number;
        bd = b->number;

        if      (ad == bd) { r =  0; }
        else if (ad <  bd) { r = -1; }
        else               { r =  1; }
    } else {
        JULE_ASSERT(arg->sort_type == JULE_STRING);

        ac = bc = NULL;
        as = a->type == JULE_STRING
                ? jule_get_string(arg->interp, a->string_id)->chars
                : (ac = jule_to_string(arg->interp, a, JULE_NO_QUOTE));
        bs = b->type == JULE_STRING
                ? jule_get_string(arg->interp, b->string_id)->chars
                : (bc = jule_to_string(arg->interp, b, JULE_NO_QUOTE));

        r = strcmp(as, bs);

        if (ac != NULL) { JULE_FREE(ac); }
        if (bc != NULL) { JULE_FREE(bc); }
    }

    return r;
}

static Jule_Status jule_builtin_sorted(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status     status;
    Jule_Value     *list;
    Jule_Value     *sorted;
    Jule_Type       sort_type;
    Jule_Value     *it;
    _Jule_Sort_Arg  sort_arg;

    status = jule_args(interp, tree, "l", n_values, values, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    sorted = jule_copy_force(list);

    if (jule_len(sorted->list) > 0) {
        sort_type = JULE_UNKNOWN;

        FOR_EACH(sorted->list, it) {
            if (JULE_TYPE_IS_KEYLIKE(it->type)) {
                if (it->type == JULE_STRING) {
                    sort_type = JULE_STRING;
                } else if (sort_type != JULE_STRING) {
                    sort_type = JULE_NUMBER;
                }
            } else {
                status = JULE_ERR_TYPE;
                jule_make_type_error(interp, list, _JULE_KEYLIKE, it->type);
                *result = NULL;
                jule_free_value(sorted);
                goto out_free;
            }
        }

        JULE_ASSERT(sort_type);

        sort_arg.interp    = interp;
        sort_arg.sort_type = sort_type;

        sort_r(sorted->list->data, jule_len(sorted->list), sizeof(*(sorted->list->data)), jule_sort_value_cmp, &sort_arg);
    }

    *result = sorted;

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_map(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *f;
    Jule_Value  *list;
    Jule_Value  *t;
    Jule_Value  *mapped;
    Jule_Value  *it;
    Jule_Value  *ev;

    status = jule_args(interp, tree, "*l", n_values, values, &f, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    t = ((tree->type == _JULE_TREE || tree->type == _JULE_TREE_LINE_LEADER) && jule_len(tree->eval_values) > 1)
            ? jule_elem(tree->eval_values, 1)
            : tree;

    mapped = jule_list_value();

    FOR_EACH(list->list, it) {
        status = jule_invoke(interp, t, f, 1, &it, &ev);
        if (status != JULE_SUCCESS) {
            jule_free_value(mapped);
            *result = NULL;
            goto out_free;
        }
        if (ev->in_symtab || ev->borrower_count) {
            /* Make sure we get a value that can be owned by our new list. */
            ev = jule_copy_force(ev);
        }
        mapped->list = jule_push(mapped->list, ev);
    }

    *result = mapped;

out_free:;
    jule_free_value(list);
    jule_free_value(f);

out:;
    return status;
}

static Jule_Status jule_builtin_filter(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *f;
    Jule_Value  *list;
    Jule_Value  *t;
    Jule_Value  *filtered;
    Jule_Value  *it;
    Jule_Value  *ev;

    status = jule_args(interp, tree, "*l", n_values, values, &f, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    t = ((tree->type == _JULE_TREE || tree->type == _JULE_TREE_LINE_LEADER) && jule_len(tree->eval_values) > 1)
            ? jule_elem(tree->eval_values, 1)
            : tree;

    filtered = jule_list_value();

    FOR_EACH(list->list, it) {
        status = jule_invoke(interp, t, f, 1, &it, &ev);
        if (status != JULE_SUCCESS) {
            jule_free_value(filtered);
            *result = NULL;
            goto out_free;
        }
        if (ev->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, ev, JULE_NUMBER, ev->type);
            jule_free_value(ev);
            jule_free_value(filtered);
            *result = NULL;
            goto out_free;
        }
        if (ev->number != 0) {
            filtered->list = jule_push(filtered->list, jule_copy_force(it));
        }
        jule_free_value(ev);
    }

    *result = filtered;

out_free:;
    jule_free_value(list);
    jule_free_value(f);

out:;
    return status;
}

static Jule_Status jule_builtin_reduce(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *f;
    Jule_Value  *acc;
    Jule_Value  *list;
    Jule_Value  *t;
    Jule_Value  *it;
    Jule_Value  *arg_pass[2];
    Jule_Value  *ev;

    status = jule_args(interp, tree, "**l", n_values, values, &f, &acc, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    t = ((tree->type == _JULE_TREE || tree->type == _JULE_TREE_LINE_LEADER) && jule_len(tree->eval_values) > 1)
            ? jule_elem(tree->eval_values, 1)
            : tree;

    FOR_EACH(list->list, it) {
        arg_pass[0] = acc;
        arg_pass[1] = it;
        status = jule_invoke(interp, t, f, 2, arg_pass, &ev);
        if (status != JULE_SUCCESS) {
            jule_free_value(acc);
            *result = NULL;
            goto out_free;
        }
        jule_free_value(acc);
        acc = ev;
    }

    *result = acc;

out_free:;
    jule_free_value(list);
    jule_free_value(f);

out:;
    return status;
}

static Jule_Status jule_builtin_apply(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *f;
    Jule_Value  *list;
    Jule_Value  *t;
    Jule_Value  *ev;

    status = jule_args(interp, tree, "*l", n_values, values, &f, &list);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    t = ((tree->type == _JULE_TREE || tree->type == _JULE_TREE_LINE_LEADER) && jule_len(tree->eval_values) > 1)
            ? jule_elem(tree->eval_values, 1)
            : tree;

    status = jule_invoke(interp, t, f, jule_len(list->list), (Jule_Value**)list->list->data, &ev);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free;
    }

    *result = ev;

out_free:;
    jule_free_value(list);
    jule_free_value(f);

out:;
    return status;
}

static Jule_Status jule_parse_nodes(Jule_Interp *interp, const char *str, int size, Jule_Array **out_nodes);

static Jule_Status jule_builtin_eval_file(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *path;
    const Jule_String *pstring;
    Jule_String_ID     save_file;
    const char        *mem;
    int                size;
    Jule_Array        *nodes = JULE_ARRAY_INIT;
    unsigned           i;
    Jule_Value        *it;
    Jule_Value        *ev;

    status = jule_args(interp, tree, "s", n_values, values, &path);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    pstring = jule_get_string(interp, path->string_id);
    jule_free_value(path);

    status = jule_map_file_into_readonly_memory(pstring->chars, &mem, &size);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_file_error(interp, tree, status, pstring->chars);
        goto out_free;
    }

    save_file        = interp->cur_file;
    interp->cur_file = pstring;

    status = jule_parse_nodes(interp, mem, size, &nodes);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_restore_file;
    }

    i = 0;
    FOR_EACH(nodes, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_restore_file;
        }

        i += 1;

        if (i == jule_len(nodes)) {
            *result = ev;
        } else {
            jule_free_value(ev);
        }
    }

    if (*result != NULL) {
        *result = jule_copy_force(*result);
    } else {
        *result = jule_nil_value();
    }

out_restore_file:;
    interp->cur_file = save_file;

out_free:;
    FOR_EACH(nodes, it) {
        jule_free_value_force(it);
    }
    jule_free_array(nodes);

out:;
    return status;
}

static Jule_Status jule_builtin_use_package(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *name;
    const Jule_String *name_string;

    if (interp->use_package_forbidden) {
        *result = NULL;
        status  = JULE_ERR_USE_PACKAGE_FORBIDDEN;
        jule_make_forbidden_error(interp, tree, status);
        goto out;
    }

    status = jule_args(interp, tree, "s", n_values, values, &name);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    name_string = jule_get_string(interp, name->string_id);
    jule_free_value(name);

    status = jule_load_package(interp, name_string->chars, result);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        jule_make_load_package_error(interp, tree, status, name_string->chars, dlerror());
        goto out;
    }

out:;
    return status;
}

static Jule_Status jule_builtin_add_package_directory(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *path;
    const Jule_String *pstring;

    if (interp->add_package_directory_forbidden) {
        *result = NULL;
        status  = JULE_ERR_USE_PACKAGE_FORBIDDEN;
        jule_make_forbidden_error(interp, tree, status);
        goto out;
    }

    status = jule_args(interp, tree, "s", n_values, values, &path);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    pstring = jule_get_string(interp, path->string_id);
    jule_free_value(path);

    jule_add_package_directory(interp, pstring->chars);

    *result = jule_string_value(interp, pstring->chars);

out:;
    return status;
}

static Jule_Status jule_builtin_exit(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *exit_code;
    int          code;

    (void)tree;

    *result   = NULL;
    exit_code = NULL;

    if (n_values >= 1) {
        status = jule_eval(interp, values[0], &exit_code);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (exit_code->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, exit_code, JULE_NUMBER, exit_code->type);
            goto out_free;
        }
    }

    code = exit_code != NULL ? (int)exit_code->number : 0;

    exit(code);

out_free:;
    if (exit_code != NULL) {
        jule_free_value(exit_code);
    }

out:;
    return status;
}

static Jule_Status jule_builtin_len(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status        status;
    Jule_Value        *ev;
    const Jule_String *string;

    status = jule_args(interp, tree, "*", n_values, values, &ev);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    switch (ev->type) {
        case JULE_NIL:
            *result = jule_number_value(0);
            break;
        case JULE_NUMBER:
            *result = jule_copy(ev);
            break;
        case JULE_STRING:
            string  = jule_get_string(interp, ev->string_id);
            *result = jule_number_value(string->len);
            break;
        case JULE_SYMBOL:
            string  = jule_get_string(interp, ev->symbol_id);
            *result = jule_number_value(string->len);
            break;
        case JULE_LIST:
            *result = jule_number_value(jule_len(ev->list));
            break;
        case JULE_OBJECT:
            *result = jule_number_value(hash_table_len((_Jule_Object)ev->object));
            break;
        case _JULE_TREE:
        case _JULE_TREE_LINE_LEADER:
        case _JULE_LAMBDA:
            *result = jule_number_value(jule_len(ev->eval_values));
            break;
        case _JULE_BUILTIN_FN:
            *result = jule_number_value(0);
            break;
        default:
            JULE_ASSERT(0);
            break;
    }

    jule_free_value(ev);

out:;
    return status;
}

static Jule_Status jule_builtin_empty(Jule_Interp *interp, Jule_Value *tree, unsigned n_values, Jule_Value **values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *ev;

    status = jule_args(interp, tree, "#", n_values, values, &ev);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (ev->type == JULE_LIST) {
        *result = jule_number_value(jule_len(ev->list) == 0);
    } else if (ev->type == JULE_OBJECT) {
        *result = jule_number_value(hash_table_len((_Jule_Object)ev->object) == 0);
    }

    jule_free_value(ev);

out:;
    return status;
}

Jule_Status jule_init_interp(Jule_Interp *interp) {
    memset(interp, 0, sizeof(*interp));

    interp->roots        = JULE_ARRAY_INIT;
    interp->strings      = hash_table_make_e(Char_Ptr, Jule_String_ID, jule_charptr_hash, jule_charptr_equ);
    interp->symtab       = hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash);
    jule_pushlocal_symtab(interp, hash_table_make(Jule_String_ID, Jule_Value_Ptr, jule_string_id_hash));
    interp->iter_vals    = JULE_ARRAY_INIT;

#define JULE_INSTALL_FN(_name, _fn) jule_install_fn(interp, jule_get_string_id(interp, (_name)), (_fn))

    JULE_INSTALL_FN("eval",                  jule_builtin_eval);
    JULE_INSTALL_FN("set",                   jule_builtin_set);
    JULE_INSTALL_FN("local",                 jule_builtin_local);
    JULE_INSTALL_FN("ref",                   jule_builtin_ref);
    JULE_INSTALL_FN("eset",                  jule_builtin_eset);
    JULE_INSTALL_FN("elocal",                jule_builtin_elocal);
    JULE_INSTALL_FN("eref",                  jule_builtin_eref);
    JULE_INSTALL_FN("fn",                    jule_builtin_fn);
    JULE_INSTALL_FN("localfn",               jule_builtin_localfn);
    JULE_INSTALL_FN("lambda",                jule_builtin_lambda);
    JULE_INSTALL_FN("id",                    jule_builtin_id);
    JULE_INSTALL_FN("`",                     jule_builtin_id);
    JULE_INSTALL_FN("quote",                 jule_builtin_quote);
    JULE_INSTALL_FN("'",                     jule_builtin_quote);
    JULE_INSTALL_FN("+",                     jule_builtin_add);
    JULE_INSTALL_FN("-",                     jule_builtin_sub);
    JULE_INSTALL_FN("*",                     jule_builtin_mul);
    JULE_INSTALL_FN("/",                     jule_builtin_div);
    JULE_INSTALL_FN("//",                    jule_builtin_idiv);
    JULE_INSTALL_FN("%",                     jule_builtin_mod);
    JULE_INSTALL_FN("++",                    jule_builtin_inc);
    JULE_INSTALL_FN("--",                    jule_builtin_dec);
    JULE_INSTALL_FN("==",                    jule_builtin_equ);
    JULE_INSTALL_FN("!=",                    jule_builtin_neq);
    JULE_INSTALL_FN("<",                     jule_builtin_lss);
    JULE_INSTALL_FN("<=",                    jule_builtin_leq);
    JULE_INSTALL_FN(">",                     jule_builtin_gtr);
    JULE_INSTALL_FN(">=",                    jule_builtin_geq);
    JULE_INSTALL_FN("not",                   jule_builtin_not);
    JULE_INSTALL_FN("and",                   jule_builtin_and);
    JULE_INSTALL_FN("or",                    jule_builtin_or);
    JULE_INSTALL_FN("print",                 jule_builtin_print);
    JULE_INSTALL_FN("println",               jule_builtin_println);
    JULE_INSTALL_FN("string",                jule_builtin_string);
    JULE_INSTALL_FN("symbol",                jule_builtin_symbol);
    JULE_INSTALL_FN("pad",                   jule_builtin_pad);
    JULE_INSTALL_FN("fmt",                   jule_builtin_fmt);
    JULE_INSTALL_FN("num-fmt",               jule_builtin_num_fmt);
    JULE_INSTALL_FN("parse-int",             jule_builtin_parse_int);
    JULE_INSTALL_FN("parse-hex",             jule_builtin_parse_hex);
    JULE_INSTALL_FN("parse-float",           jule_builtin_parse_float);
    JULE_INSTALL_FN("do",                    jule_builtin_do);
    JULE_INSTALL_FN("if",                    jule_builtin_if);
    JULE_INSTALL_FN("elif",                  jule_builtin_elif);
    JULE_INSTALL_FN("else",                  jule_builtin_else);
    JULE_INSTALL_FN("select",                jule_builtin_select);
    JULE_INSTALL_FN("while",                 jule_builtin_while);
    JULE_INSTALL_FN("foreach",               jule_builtin_foreach);
    JULE_INSTALL_FN("list",                  jule_builtin_list);
    JULE_INSTALL_FN("range",                 jule_builtin_range);
    JULE_INSTALL_FN(".",                     jule_builtin_dot);
    JULE_INSTALL_FN("elem",                  jule_builtin_elem);
    JULE_INSTALL_FN("index",                 jule_builtin_index);
    JULE_INSTALL_FN("append",                jule_builtin_append);
    JULE_INSTALL_FN("pop",                   jule_builtin_pop);
    JULE_INSTALL_FN("object",                jule_builtin_object);
    JULE_INSTALL_FN("in",                    jule_builtin_in);
    JULE_INSTALL_FN("field",                 jule_builtin_field);
    JULE_INSTALL_FN("insert",                jule_builtin_insert);
    JULE_INSTALL_FN("delete",                jule_builtin_delete);
    JULE_INSTALL_FN("update-object",         jule_builtin_update_object);
    JULE_INSTALL_FN("erase",                 jule_builtin_erase);
    JULE_INSTALL_FN("len",                   jule_builtin_len);
    JULE_INSTALL_FN("empty",                 jule_builtin_empty);
    JULE_INSTALL_FN("keys",                  jule_builtin_keys);
    JULE_INSTALL_FN("values",                jule_builtin_values);
    JULE_INSTALL_FN("sorted",                jule_builtin_sorted);
    JULE_INSTALL_FN("map",                   jule_builtin_map);
    JULE_INSTALL_FN("filter",                jule_builtin_filter);
    JULE_INSTALL_FN("reduce",                jule_builtin_reduce);
    JULE_INSTALL_FN("apply",                 jule_builtin_apply);
    JULE_INSTALL_FN("eval-file",             jule_builtin_eval_file);
    JULE_INSTALL_FN("use-package",           jule_builtin_use_package);
    JULE_INSTALL_FN("add-package-directory", jule_builtin_add_package_directory);
    JULE_INSTALL_FN("exit",                  jule_builtin_exit);

#undef JULE_INSTALL_FN

    return JULE_SUCCESS;
}

Jule_Status jule_interp(Jule_Interp *interp) {
    Jule_Status  status;
    Jule_Value  *root;
    Jule_Value  *result;

    status = JULE_SUCCESS;

    if (jule_len(interp->roots) == 0) {
        return JULE_ERR_NO_INPUT;
    }

    FOR_EACH(interp->roots, root) {
        status = jule_eval(interp, root, &result);
        if (status != JULE_SUCCESS) {
            goto out;
        }
        jule_free_value(result);
    }

out:;
    return status;
}

void jule_free(Jule_Interp *interp) {
    _Jule_Symbol_Table    symtab;
    Jule_Value           *it;
    char                 *key;
    Jule_String_ID       *id;
    void                 *handle;
    Jule_Backtrace_Entry *bt;


    while ((symtab = jule_pop(interp->local_symtab_stack)) != NULL) {
        jule_free_symtab(symtab);
    }
    jule_free_array(interp->local_symtab_stack);

    jule_free_symtab(interp->symtab);

    FOR_EACH(interp->package_values, it) {
        jule_free_value_force(it);
    }
    jule_free_array(interp->package_values);

    FOR_EACH(interp->roots, it) {
        jule_free_value_force(it);
    }
    jule_free_array(interp->roots);

    hash_table_traverse(interp->strings, key, id) {
        (void)key;
        jule_free_string((Jule_String*)jule_get_string(interp, *id));
        JULE_FREE((void*)*id);
    }
    hash_table_free(interp->strings);

    FOR_EACH(interp->package_handles, handle) {
        dlclose(handle);
    }
    jule_free_array(interp->package_handles);

    jule_free_array(interp->package_dirs);

    FOR_EACH(interp->backtrace, bt) {
        JULE_FREE(bt);
    }
    jule_free_array(interp->backtrace);

    memset(interp, 0, sizeof(*interp));
}

#undef STATUS_ERR_RET
#undef PARSE_ERR_RET
#undef MORE_INPUT
#undef PEEK_CHAR
#undef SPC
#undef DIG

#undef STR
#undef _STR
#undef CAT2
#undef _CAT2
#undef CAT3
#undef _CAT3
#undef CAT4
#undef _CAT4
#undef hash_table
#undef hash_table_make
#undef hash_table_make_e
#undef hash_table_len
#undef hash_table_free
#undef hash_table_get_key
#undef hash_table_get_val
#undef hash_table_insert
#undef hash_table_delete
#undef hash_table_traverse
#undef _hash_table_slot
#undef hash_table_slot
#undef _hash_table
#undef hash_table
#undef hash_table_pretty_name
#undef _HASH_TABLE_EQU
#undef DEFAULT_START_SIZE_IDX
#undef use_hash_table

#endif /* JULE_IMPL */

#endif /* __JULE_H__ */
