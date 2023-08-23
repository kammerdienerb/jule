#ifndef __JULE_H__
#define __JULE_H__

#define _JULE_STATUS                                                                 \
    _JULE_STATUS_X(JULE_SUCCESS,              "No error.")                           \
    _JULE_STATUS_X(JULE_ERR_UNEXPECTED_EOS,   "Unexpected end of input.")            \
    _JULE_STATUS_X(JULE_ERR_UNEXPECTED_TOK,   "Unexpected token.")                   \
    _JULE_STATUS_X(JULE_ERR_NO_INPUT,         "Missing a top-level expression.")     \
    _JULE_STATUS_X(JULE_ERR_LOOKUP,           "Failed to find symbol.")              \
    _JULE_STATUS_X(JULE_ERR_NOT_A_FN,         "Invoked value is not a function.")    \
    _JULE_STATUS_X(JULE_ERR_ARITY,            "Incorrect number of arguments.")      \
    _JULE_STATUS_X(JULE_ERR_TYPE,             "Incorrect argument type.")            \
    _JULE_STATUS_X(JULE_ERR_OBJECT_KEY_TYPE,  "Expression is not a valid key type.") \
    _JULE_STATUS_X(JULE_ERR_MISSING_VAL,      "Missing value expression.")           \
    _JULE_STATUS_X(JULE_ERR_BAD_INDEX,        "Field or element not found.")         \
    _JULE_STATUS_X(JULE_ERR_EVAL_CANCELLED,   "Evaluation was cancelled.")

#define _JULE_STATUS_X(e, s) e,
typedef enum { _JULE_STATUS } Jule_Status;
#undef _JULE_STATUS_X

#define _JULE_TYPE                                             \
    _JULE_TYPE_X(JULE_UNKNOWN,           "<unknown type>")     \
    _JULE_TYPE_X(JULE_NIL,               "nil")                \
    _JULE_TYPE_X(JULE_NUMBER,            "number")             \
    _JULE_TYPE_X(JULE_STRING,            "string")             \
    _JULE_TYPE_X(JULE_SYMBOL,            "symbol")             \
    _JULE_TYPE_X(JULE_LIST,              "list")               \
    _JULE_TYPE_X(JULE_OBJECT,            "object")             \
    _JULE_TYPE_X(_JULE_TREE,             "function")           \
    _JULE_TYPE_X(_JULE_BUILTIN_FN,       "function (builtin)") \
    _JULE_TYPE_X(_JULE_LIST_OR_OBJECT,   "list or object")     \
    _JULE_TYPE_X(_JULE_STRING_OR_NUMBER, "string or number")

#define _JULE_TYPE_X(e, s) e,
typedef enum { _JULE_TYPE } Jule_Type;
#undef _JULE_TYPE_X

enum {
    JULE_NO_QUOTE  = 1u << 0u,
    JULE_MULTILINE = 1u << 1u,
};

typedef void *Jule_Object;

struct Jule_Value_Struct;
typedef struct Jule_Value_Struct Jule_Value;

struct Jule_String_Struct;
typedef struct Jule_String_Struct Jule_String;

struct Jule_Array_Struct;
typedef struct Jule_Array_Struct Jule_Array;

struct Jule_Interp_Struct;
typedef struct Jule_Interp_Struct Jule_Interp;

typedef struct {
    int line;
    int col;
} Jule_Parse_Location;

typedef struct {
    Jule_Status          status;
    Jule_Parse_Location  location;
    char                *sym;
    Jule_Type            wanted_type;
    Jule_Type            got_type;
    int                  arity_at_least;
    int                  wanted_arity;
    int                  got_arity;
    Jule_Value          *bad_index;
} Jule_Error_Info;

typedef void (*Jule_Error_Callback)(Jule_Error_Info *info);
typedef void (*Jule_Output_Callback)(const char*, int);
typedef Jule_Status (*Jule_Eval_Callback)(Jule_Value *value);

typedef Jule_Status (*Jule_Fn)(Jule_Interp*, Jule_Value*, Jule_Array, Jule_Value**);

const char  *jule_error_string(Jule_Status error);
const char  *jule_type_string(Jule_Type type);
char        *jule_to_string(Jule_Value *value, int flags);
Jule_Status  jule_init_interp(Jule_Interp *interp);
Jule_Status  jule_set_error_callback(Jule_Interp *interp, Jule_Error_Callback cb);
Jule_Status  jule_set_output_callback(Jule_Interp *interp, Jule_Output_Callback cb);
Jule_Status  jule_set_eval_callback(Jule_Interp *interp, Jule_Eval_Callback cb);
void         jule_free_error_info(Jule_Error_Info *info);
Jule_Status  jule_parse(Jule_Interp *interp, const char *str, int size);
Jule_Status  jule_interp(Jule_Interp *interp);
Jule_Value  *jule_nil_value(void);
Jule_Value  *jule_number_value(double num);
Jule_Value  *jule_string_value(const char *str, unsigned long long len);
Jule_Value  *jule_symbol_value(const char *symbol, int len);
Jule_Value  *jule_list_value(void);
Jule_Value  *jule_builtin_value(Jule_Fn fn);
Jule_Value  *jule_object_value(void);
void         jule_insert(Jule_Value *object, Jule_Value *key, Jule_Value *val);
void         jule_delete(Jule_Value *object, Jule_Value *key);
Jule_Value  *jule_lookup(Jule_Interp *interp, const char *symbol);
Jule_Status  jule_install_var(Jule_Interp *interp, const char *symbol, Jule_Value *val);
Jule_Status  jule_install_fn(Jule_Interp *interp, const char *symbol, Jule_Fn fn);
Jule_Status  jule_install_local(Jule_Interp *interp, const char *symbol, Jule_Value *val);
Jule_Status  jule_uninstall_var(Jule_Interp *interp, const char *symbol);
Jule_Status  jule_uninstall_fn(Jule_Interp *interp, const char *symbol);
Jule_Status  jule_uninstall_local(Jule_Interp *interp, const char *symbol);
void         jule_free(Jule_Interp *interp);


#ifdef JULE_IMPL

#include <assert.h>

#include <stdint.h>
#include <string.h> /* strlen, memcpy, memset, memcmp */
#include <stdarg.h>

#ifndef JULE_MALLOC
#include <stdlib.h>
#define JULE_MALLOC (malloc)
#endif
#ifndef JULE_REALLOC
#include <stdlib.h>
#define JULE_REALLOC (realloc)
#endif
#ifndef JULE_FREE
#include <stdlib.h>
#define JULE_FREE (free)
#endif

struct Jule_String_Struct {
    char               *chars;
    unsigned long long  len;
};

static inline void jule_free_string(Jule_String *string) {
    JULE_FREE(string->chars);
    string->chars = NULL;
    string->len   = 0;
}

static inline Jule_String jule_string(const char *s, unsigned long long len) {
    Jule_String string;

    string.len   = len;
    string.chars = JULE_MALLOC(string.len);
    memcpy(string.chars, s, string.len);

    return string;
}

static inline Jule_String jule_strdup(Jule_String *s) {
    return jule_string(s->chars, s->len);
}

static inline Jule_String jule_concat(Jule_String *s1, Jule_String *s2) {
    Jule_String string;

    string.len   = s1->len + s2->len;
    string.chars = JULE_MALLOC(string.len);
    memcpy(string.chars, s1->chars, s1->len);
    memcpy(string.chars + s1->len, s2->chars, s2->len);

    return string;
}

struct Jule_Array_Struct {
    void     **data;
    unsigned   len;
    unsigned   cap;
};

#define JULE_ARRAY_INIT ((Jule_Array){0})

static inline void jule_free_array(Jule_Array *array) {
    if (array->data != NULL) {
        JULE_FREE(array->data);
    }
    memset(array, 0, sizeof(*array));
}

static inline void jule_push(Jule_Array *array, void *item) {
    if (array->data == NULL) {
        array->len = 0;
        array->cap = 4;
        goto alloc;
    }

    if (array->len >= array->cap) {
        array->cap += ((array->cap >> 1) > 0) ? (array->cap >> 1) : 1;
alloc:;
        array->data = JULE_REALLOC(array->data, array->cap * sizeof(*array->data));
    }
    array->data[array->len] = item;
    array->len += 1;
}

static inline void jule_pop(Jule_Array *array) {
    if (array->len > 0) {
        array->len -= 1;
    }
}

static inline void *jule_elem(Jule_Array *array, unsigned idx) {
    if (idx < 0 || idx >= array->len) {
        return NULL;
    }

    return array->data[idx];
}

static inline void *jule_top(Jule_Array *array) {
    return jule_elem(array, array->len - 1);
}

static inline void *jule_last(Jule_Array *array) {
    return jule_elem(array, array->len - 1);
}

#define FOR_EACH(_arrayp, _it) for (unsigned _each_i = 0; (((_arrayp)->len && ((_it) = (_arrayp)->data[_each_i])), _each_i < (_arrayp)->len); _each_i += 1)

struct Jule_Value_Struct {
    union {
        unsigned long long  _integer;
        double              number;
        Jule_String         string;
        char               *symbol;
        Jule_Object         object;
        Jule_Array          list;
        Jule_Array          eval_values;
        Jule_Fn             builtin_fn;
    };
    unsigned                type      :  4;
    unsigned                code      :  1;
    unsigned                in_symtab :  1;
    unsigned                local     :  1;
    unsigned                ind_level :  9;
    unsigned                line      : 16;
};

typedef struct Jule_Parse_Context_Struct {
    Jule_Interp *interp;
    const char  *cursor;
    const char  *end;
    Jule_Array   stack;
    Jule_Array   roots;
    int          line;
} Jule_Parse_Context;

typedef char *Char_Ptr;

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
    CAT2(hash_table(K_T, V_T), _make)(CAT2(hash_table(K_T, V_T), _hash_t) hash, void *equ) { \
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

static unsigned long long jule_string_hash(Jule_String *s) {
    unsigned long long i;
    int c;
    unsigned long hash = 5381;

    for (i = 0; i < s->len; i += 1) {
        c = s->chars[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

static int jule_string_equ(Jule_String *s1, Jule_String *s2) {
    if (s1->len == s2->len) {
        return strncmp(s1->chars, s2->chars, s1->len) == 0;
    }

    return 0;
}

static int jule_equal(Jule_Value *a, Jule_Value *b);

static unsigned long long jule_valhash(Jule_Value *val) {
    assert(val->type == JULE_NUMBER || val->type == JULE_STRING);

    /* @todo zeros, nan, inf w/ sign */
    if (val->type == JULE_NUMBER) {
        return val->_integer;
    }

    return jule_string_hash(&val->string);
}


typedef Jule_Value *Jule_Value_Ptr;

use_hash_table(Char_Ptr, Jule_Value_Ptr)

typedef hash_table(Char_Ptr, Jule_Value_Ptr) _Jule_Symbol_Table;

use_hash_table(Jule_Value_Ptr, Jule_Value_Ptr)

typedef hash_table(Jule_Value_Ptr, Jule_Value_Ptr) _Jule_Object;


struct Jule_Interp_Struct {
    Jule_Array           roots;
    Jule_Error_Callback  error_callback;
    Jule_Output_Callback output_callback;
    Jule_Eval_Callback   eval_callback;
    _Jule_Symbol_Table   symtab;
    _Jule_Symbol_Table   local_symtab;
};


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

static void jule_free_value(Jule_Value *value);


void jule_free_error_info(Jule_Error_Info *info) {
    if (info->sym != NULL) {
        JULE_FREE(info->sym);
    }
    if (info->bad_index != NULL) {
        jule_free_value(info->bad_index);
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

Jule_Value *jule_string_value(const char *str, unsigned long long len) {
    Jule_Value *value;

    value = _jule_value();

    value->type   = JULE_STRING;
    value->string = jule_string(str, len);

    return value;
}

Jule_Value *jule_symbol_value(const char *symbol, int len) {
    Jule_Value *value;

    value = _jule_value();

    value->type   = JULE_SYMBOL;
    value->symbol = jule_charptr_ndup(symbol, len);

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
    value->object = hash_table_make_e(Jule_Value_Ptr, Jule_Value_Ptr, jule_valhash, (void*)jule_equal);

    return value;
}

static void jule_free_value(Jule_Value *value) {
    Jule_Value  *child;
    Jule_Value  *key;
    Jule_Value **val;

    if (value->in_symtab || value->code) { return; }

    switch (value->type) {
        case JULE_NIL:
            break;
        case JULE_NUMBER:
            break;
        case JULE_STRING:
            jule_free_string(&value->string);
            break;
        case JULE_SYMBOL:
            JULE_FREE(value->symbol);
            break;
        case JULE_LIST:
            FOR_EACH(&value->list, child) {
                jule_free_value(child);
            }
            jule_free_array(&value->list);
            break;
        case JULE_OBJECT:
            hash_table_traverse((_Jule_Object)value->object, key, val) {
                jule_free_value(key);
                jule_free_value(*val);
            }
            hash_table_free((_Jule_Object)value->object);
            value->object = NULL;
            break;
        case _JULE_TREE:
            FOR_EACH(&value->eval_values, child) {
                jule_free_value(child);
            }
            jule_free_array(&value->eval_values);
            break;
        case _JULE_BUILTIN_FN:
            break;
        default:
            assert(0);
            break;
    }

    JULE_FREE(value);
}

void jule_insert(Jule_Value *object, Jule_Value *key, Jule_Value *val) {
    Jule_Value **lookup;

    lookup = hash_table_get_val((_Jule_Object)object->object, key);
    if (lookup != NULL) {
        jule_free_value(key);
        jule_free_value(*lookup);
        *lookup = val;
    } else {
        hash_table_insert((_Jule_Object)object->object, key, val);
    }
}

void jule_delete(Jule_Value *object, Jule_Value *key) {
    Jule_Value **lookup;
    Jule_Value  *real_key;
    Jule_Value  *val;

    lookup = hash_table_get_key((_Jule_Object)object->object, key);

    if (lookup != NULL) {
        real_key = *lookup;
        val      = *hash_table_get_val((_Jule_Object)object->object, real_key);
        hash_table_delete((_Jule_Object)object->object, real_key);
        jule_free_value(real_key);
        jule_free_value(val);
    }
}

static Jule_Value *_jule_copy(Jule_Value *value, int force) {
    Jule_Value    *copy;
    Jule_Array     array = JULE_ARRAY_INIT;
    Jule_Value    *child;
    _Jule_Object   obj;
    Jule_Value    *key;
    Jule_Value   **val;

    if (!force && (value->in_symtab || value->code)) { return value; }

    copy = _jule_value();
    memcpy(copy, value, sizeof(*copy));

    switch (value->type) {
        case JULE_NIL:
            break;
        case JULE_NUMBER:
            break;
        case JULE_STRING:
            copy->string = jule_strdup(&copy->string);
            break;
        case JULE_SYMBOL:
            copy->symbol = jule_charptr_dup(copy->symbol);
            break;
        case JULE_LIST:
            FOR_EACH(&copy->list, child) {
                jule_push(&array, _jule_copy(child, force));
            }
            copy->list = array;
            break;
        case JULE_OBJECT:
            obj = copy->object;
            copy->object = hash_table_make_e(Jule_Value_Ptr, Jule_Value_Ptr, jule_valhash, (void*)jule_equal);
            hash_table_traverse(obj, key, val) {
                hash_table_insert((_Jule_Object)copy->object, _jule_copy(key, force), _jule_copy(*val, force));
            }
            break;
        case _JULE_TREE:
            FOR_EACH(&copy->eval_values, child) {
                jule_push(&array, _jule_copy(child, force));
            }
            copy->eval_values = array;
            break;
        case _JULE_BUILTIN_FN:
            break;
        default:
            assert(0);
            break;
    }

    if (force) {
        copy->in_symtab = 0;
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
            return jule_string_equ(&a->string, &b->string);
        case JULE_SYMBOL:
            return strcmp(a->symbol, b->symbol) == 0;
        case JULE_LIST:
            if (a->list.len != b->list.len) { return 0; }
            for (i = 0; i < a->list.len; i += 1) {
                ia = jule_elem(&a->list, i);
                ib = jule_elem(&b->list, i);
                if (!jule_equal(ia, ib)) { return 0; }
            }
            return 1;
        case JULE_OBJECT:
            assert(0);
            return 0;
        default:
            assert(0);
            break;
    }

    return 0;
}

static void jule_error(Jule_Interp *interp, Jule_Error_Info *info) {
    if (interp->error_callback != NULL) {
        interp->error_callback(info);
    }
}

static void jule_make_parse_error(Jule_Interp *interp, int line, int col, Jule_Status status) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = status;
    info.location.line = line;
    info.location.col  = col;
    jule_error(interp, &info);
}

static void jule_make_interp_error(Jule_Interp *interp, int line, Jule_Status status) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = status;
    info.location.line = line;
    info.location.col  = 0;
    jule_error(interp, &info);
}

static void jule_make_lookup_error(Jule_Interp *interp, int line, const char *sym) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = JULE_ERR_LOOKUP;
    info.location.line = line;
    info.location.col  = 0;
    info.sym           = jule_charptr_dup(sym);
    jule_error(interp, &info);
}

static void jule_make_arity_error(Jule_Interp *interp, int line, int wanted, int got, int at_least) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status         = JULE_ERR_ARITY;
    info.location.line  = line;
    info.location.col   = 0;
    info.wanted_arity   = wanted;
    info.got_arity      = got;
    info.arity_at_least = at_least;
    jule_error(interp, &info);
}

static void jule_make_type_error(Jule_Interp *interp, int line, Jule_Type wanted, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = JULE_ERR_TYPE;
    info.location.line = line;
    info.location.col  = 0;
    info.wanted_type   = wanted;
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_object_key_type_error(Jule_Interp *interp, int line, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = JULE_ERR_OBJECT_KEY_TYPE;
    info.location.line = line;
    info.location.col  = 0;
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_not_a_fn_error(Jule_Interp *interp, int line, Jule_Type got) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = JULE_ERR_NOT_A_FN;
    info.location.line = line;
    info.location.col  = 0;
    info.got_type      = got;
    jule_error(interp, &info);
}

static void jule_make_bad_index_error(Jule_Interp *interp, int line, Jule_Value *bad_index) {
    Jule_Error_Info info;
    memset(&info, 0, sizeof(info));
    info.status        = JULE_ERR_BAD_INDEX;
    info.location.line = line;
    info.location.col  = 0;
    info.bad_index     = bad_index;
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

    if (c == '"') {
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
    while (PEEK_CHAR(cxt, c) && !SPC(c) && c != '#') { NEXT(cxt); }
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

static void jule_ensure_top_is_tree(Jule_Parse_Context *cxt) {
    Jule_Value *top;
    Jule_Value *value;

    top = jule_top(&cxt->stack);

    if (top->type == _JULE_TREE) { return; }

    value = _jule_value();
    memcpy(value, top, sizeof(*value));
    memset(top, 0, sizeof(*top));
    top->type      = _JULE_TREE;
    top->ind_level = value->ind_level;
    top->line      = value->line;
    jule_push(&top->eval_values, value);
}

static int jule_consume_comment(Jule_Parse_Context *cxt) {
    int c;

    if (PEEK_CHAR(cxt, c) && c == '#') {
        NEXT(cxt);
        while (PEEK_CHAR(cxt, c)) {
            NEXT(cxt);
            if (c == '\n') { break; }
        }
        return 1;
    }

    return 0;
}

static Jule_Status jule_parse_line(Jule_Parse_Context *cxt) {
    int                 ind;
    int                 c;
    Jule_Value         *top;
    int                 col;
    int                 ws;
    int                 first;
    const char         *tk_start;
    Jule_Token          tk;
    const char         *tk_end;
    Jule_Value         *val;
    char               *sbuff;
    unsigned long long  slen;
    char                d_copy[128];
    double              d;

    ind = jule_trim_leading_ws(cxt);

    if (!PEEK_CHAR(cxt, c)) {            goto done; }
    if (c == '\n')          { NEXT(cxt); goto done; }

    if (jule_consume_comment(cxt)) { goto done; }

    while ((top = jule_top(&cxt->stack)) != NULL
    &&     ind <= top->ind_level) {

        jule_pop(&cxt->stack);
    }

    col   = 1;
    ws    = ind;
    first = 1;
    do {
        col += ws;

        if (jule_consume_comment(cxt)) { goto done; }

        tk_start = cxt->cursor;
        if ((tk = jule_parse_token(cxt)) == JULE_TK_NONE) { break; }
        tk_end = cxt->cursor;

        col += tk_end - tk_start;

        switch (tk) {
            case JULE_TK_SYMBOL:
                if (tk_end - tk_start == 3 && strncmp(tk_start, "nil", tk_end - tk_start) == 0) {
                    val = jule_nil_value();
                } else {
                    val = jule_symbol_value(tk_start, tk_end - tk_start);
                }
                break;
            case JULE_TK_STRING:
                assert(tk_start[0] == '"' && "string doesn't start with quote");
                tk_start += 1;

                sbuff = alloca(tk_end - tk_start);
                slen  = 0;

                for (; tk_start < tk_end; tk_start += 1) {
                    c = *tk_start;

                    if (c == '"') { break; }
                    if (c == '\\') {
                        tk_start += 1;
                        if (tk_start < tk_end) {
                            c = *tk_start;
                            switch (c) {
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

                val = jule_string_value(sbuff, slen);
                break;
            case JULE_TK_NUMBER:
                strncpy(d_copy, tk_start, tk_end - tk_start);
                d_copy[tk_end - tk_start] = 0;
                sscanf(d_copy, "%lg", &d);
                val = jule_number_value(d);
                break;
            case JULE_TK_EOS_ERR:
                PARSE_ERR_RET(cxt->interp, JULE_ERR_UNEXPECTED_EOS, cxt->line, col);
                break;
            default:
                break;
        }

        val->ind_level = ind;
        val->line      = cxt->line;
        val->code      = 1;

        if (first) {
            if (top != NULL) {
                jule_ensure_top_is_tree(cxt);
                jule_push(&top->eval_values, val);
            } else {
                jule_push(&cxt->roots, val);
            }
            jule_push(&cxt->stack, val);
            top = val;
        } else {
            jule_ensure_top_is_tree(cxt);
            jule_push(&top->eval_values, val);
        }

        first = 0;

    } while ((ws = jule_trim_leading_ws(cxt)) > 0);

    if (jule_consume_comment(cxt)) { goto done; }

    if (PEEK_CHAR(cxt, c)) {
        if (c == '\n') {
            NEXT(cxt);
        } else {
            PARSE_ERR_RET(cxt->interp, JULE_ERR_UNEXPECTED_TOK, cxt->line, col);
        }
    }

done:;
    return JULE_SUCCESS;
}

static void _jule_string_print(char **buff, int *len, int *cap, Jule_Value *value, unsigned ind, int flags) {
    unsigned     i;
    char         b[128];
    Jule_Value  *child;
    Jule_Value  *key;
    Jule_Value **val;

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

    if (value->type != _JULE_TREE) {
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
            if (flags & JULE_NO_QUOTE) {
                PUSHSN(value->string.chars, value->string.len);
            } else {
                PUSHC('"');
                PUSHSN(value->string.chars, value->string.len);
                PUSHC('"');
            }
            break;
        case JULE_SYMBOL:
            PUSHS(value->symbol);
            break;
        case JULE_LIST:
            PUSHC('[');
            PUSHC(flags & JULE_MULTILINE ? '\n' : ' ');
            FOR_EACH(&value->list, child) {
                _jule_string_print(buff, len, cap, child, flags & JULE_MULTILINE ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC(flags & JULE_MULTILINE ? '\n' : ' ');
            }
            if (flags & JULE_MULTILINE) {
                for (i = 0; i < ind; i += 1) { PUSHC(' '); }
            }
            PUSHC(']');
            break;
        case JULE_OBJECT:
            PUSHC('{');
            PUSHC(flags & JULE_MULTILINE ? '\n' : ' ');
            hash_table_traverse((_Jule_Object)value->object, key, val) {
                _jule_string_print(buff, len, cap, key, flags & JULE_MULTILINE ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC(':');
                _jule_string_print(buff, len, cap, *val, flags & JULE_MULTILINE ? ind + 2 : 0, flags & ~JULE_NO_QUOTE);
                PUSHC(flags & JULE_MULTILINE ? '\n' : ' ');
            }
            if (flags & JULE_MULTILINE) {
                for (i = 0; i < ind; i += 1) { PUSHC(' '); }
            }
            PUSHC('}');
            break;
        case _JULE_TREE:
            _jule_string_print(buff, len, cap, value->eval_values.data[0], ind, flags & ~JULE_NO_QUOTE);
            for (i = 1; i < value->eval_values.len; i += 1) {
                PUSHC('\n');
                _jule_string_print(buff, len, cap, value->eval_values.data[i], ind + 2, flags & ~JULE_NO_QUOTE);
            }
            break;
        case _JULE_BUILTIN_FN:
            snprintf(b, sizeof(b), "<fn@%p>", (void*)value->builtin_fn);
            PUSHS(b);
            break;
        default:
            assert(0);
            break;

    }

    PUSHC(0);
    *len -= 1;
}

char *jule_to_string(Jule_Value *value, int flags) {
    char *buff;
    int   len;
    int   cap;

    buff = JULE_MALLOC(16);
    len  = 0;
    cap  = 16;

    _jule_string_print(&buff, &len, &cap, value, 0, flags);

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

    _jule_string_print(&buff, &len, &cap, value, ind, JULE_NO_QUOTE | JULE_MULTILINE);
    jule_output(interp, buff, len);
    JULE_FREE(buff);
}

Jule_Status jule_parse(Jule_Interp *interp, const char *str, int size) {
    Jule_Parse_Context cxt;
    Jule_Status        status;

    memset(&cxt, 0, sizeof(cxt));

    cxt.interp = interp;
    cxt.cursor = str;
    cxt.end    = str + size;

    status = JULE_SUCCESS;
    while (status == JULE_SUCCESS && MORE_INPUT(&cxt)) {
        cxt.line += 1;
        status    = jule_parse_line(&cxt);
    }

    interp->roots = cxt.roots;

    jule_free_array(&cxt.stack);

    return status;
}

Jule_Value *jule_lookup(Jule_Interp *interp, const char *symbol) {
    Jule_Value **lookup;

    lookup = hash_table_get_val(interp->local_symtab, (char*)symbol);

    if (lookup == NULL) {
        lookup = hash_table_get_val(interp->symtab, (char*)symbol);
    }

    return lookup == NULL ? NULL : *lookup;
}

static Jule_Status jule_install_common(Jule_Interp *interp, _Jule_Symbol_Table symtab, const char *symbol, Jule_Value *val, int local) {
    Jule_Value **lookup;

    (void)interp;

    val->in_symtab = 1;
    val->local     = !!local;

    lookup = hash_table_get_val(symtab, (char*)symbol);
    if (lookup != NULL) {
        if (*lookup != val) {
            (*lookup)->in_symtab = 0;
            jule_free_value(*lookup);
            *lookup = val;
        }
    } else {
        hash_table_insert(symtab, jule_charptr_dup(symbol), val);
    }

    return JULE_SUCCESS;
}

static Jule_Status jule_uninstall_common(Jule_Interp *interp, _Jule_Symbol_Table symtab, const char *symbol) {
    Jule_Value **lookup;
    char        *key;

    (void)interp;

    lookup = hash_table_get_val(symtab, (char*)symbol);
    if (lookup == NULL) {
        return JULE_ERR_LOOKUP;
    }

    (*lookup)->in_symtab = 0;
    jule_free_value(*lookup);
    key = *hash_table_get_key(symtab, (char*)symbol);
    JULE_FREE(key);

    return JULE_SUCCESS;
}

Jule_Status jule_install_var(Jule_Interp *interp, const char *symbol, Jule_Value *val) {
    return jule_install_common(interp, interp->symtab, symbol, val, 0);
}

Jule_Status jule_install_fn(Jule_Interp *interp, const char *symbol, Jule_Fn fn) {
    return jule_install_var(interp, symbol, jule_builtin_value(fn));
}

Jule_Status jule_install_local(Jule_Interp *interp, const char *symbol, Jule_Value *val) {
    return jule_install_common(interp, interp->local_symtab, symbol, val, 1);
}

Jule_Status jule_uninstall_var(Jule_Interp *interp, const char *symbol) {
    return jule_uninstall_common(interp, interp->symtab, symbol);
}

Jule_Status jule_uninstall_fn(Jule_Interp *interp, const char *symbol) {
    return jule_uninstall_var(interp, symbol);
}

Jule_Status jule_uninstall_local(Jule_Interp *interp, const char *symbol) {
    return jule_uninstall_common(interp, interp->local_symtab, symbol);
}

static Jule_Status jule_eval(Jule_Interp *interp, Jule_Value *value, Jule_Value **result);

static Jule_Status jule_invoke(Jule_Interp *interp, Jule_Value *tree, Jule_Value *fn, Jule_Array values, Jule_Value **result) {
    Jule_Status          status;
    Jule_Value          *def_tree;
    _Jule_Symbol_Table   save_symtab;
    _Jule_Symbol_Table   local_symtab;
    int                  i;
    Jule_Array           arg_syms;
    Jule_Value          *arg_sym;
    Jule_Value          *arg_val;
    char                *key;
    Jule_Value         **vit;
    Jule_Value          *expr;
    Jule_Value          *ev;

    status = JULE_SUCCESS;

    if (fn->type == _JULE_TREE) {
        def_tree = jule_elem(&fn->eval_values, 1);

        if (def_tree->type == _JULE_TREE) {
            arg_syms.data = def_tree->eval_values.data + 1;
            arg_syms.len  = def_tree->eval_values.len  - 1;
            arg_syms.cap  = 0;
        } else if (def_tree->type == JULE_SYMBOL) {
            arg_syms.data = NULL;
            arg_syms.len  = 0;
            arg_syms.cap  = 0;
        } else {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, def_tree->line, JULE_SYMBOL, def_tree->type);
            *result = NULL;
            goto out;
        }

        if (values.len != arg_syms.len) {
            status = JULE_ERR_ARITY;
            jule_make_arity_error(interp, tree->line, arg_syms.len, values.len, 0);
            *result = NULL;
            goto out;
        }

        save_symtab  = interp->local_symtab;
        local_symtab = hash_table_make_e(Char_Ptr, Jule_Value_Ptr, jule_charptr_hash, (void*)jule_charptr_equ);

        i = 0;
        FOR_EACH(&arg_syms, arg_sym) {
            assert(arg_sym->type == JULE_SYMBOL);

            status = jule_eval(interp, jule_elem(&values, i), &arg_val);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out_cleanup_locals;
            }

            jule_install_common(interp, local_symtab, arg_sym->symbol, jule_copy_force(arg_val), 1);

            i += 1;
        }

        interp->local_symtab = local_symtab;

        expr = jule_elem(&fn->eval_values, 2);
        status = jule_eval(interp, expr, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_cleanup_locals;
        }
        *result = ev;

        if ((*result)->local) {
            *result = jule_copy_force(*result);
        }

out_cleanup_locals:;
        hash_table_traverse(local_symtab, key, vit) {
            jule_uninstall_local(interp, key);
        }
        hash_table_free(local_symtab);

        interp->local_symtab = save_symtab;

    } else if (fn->type == _JULE_BUILTIN_FN) {
        status = fn->builtin_fn(interp, tree, values, result);
    }

out:;
    return status;
}

static Jule_Status jule_eval(Jule_Interp *interp, Jule_Value *value, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *lookup;
    Jule_Value  *fn;
    Jule_Array   values;

    status  = JULE_SUCCESS;
    *result = NULL;

    if (interp->eval_callback != NULL) {
        status = interp->eval_callback(value);
        if (status != JULE_SUCCESS) {
            jule_make_interp_error(interp, value->line, status);
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
            if ((lookup = jule_lookup(interp, value->symbol)) == NULL) {
                status = JULE_ERR_LOOKUP;
                jule_make_lookup_error(interp, value->line, value->symbol);
                *result = NULL;
                goto out;
            }
            if (lookup->type != _JULE_TREE && lookup->type != _JULE_BUILTIN_FN) {
                *result = jule_copy(lookup);
                goto out;
            }
            fn          = lookup;
            values.data = NULL;
            values.len  = 0;
            values.cap  = 0;
            goto invoke;

        case _JULE_TREE:
            assert(value->eval_values.len >= 1);

            fn = jule_elem(&value->eval_values, 0);
            if (fn->type != JULE_SYMBOL) {
                status = JULE_ERR_NOT_A_FN;
                jule_make_not_a_fn_error(interp, value->line, fn->type);
                *result = NULL;
                goto out;
            }

            if ((lookup = jule_lookup(interp, fn->symbol)) == NULL) {
                status = JULE_ERR_LOOKUP;
                jule_make_lookup_error(interp, value->line, fn->symbol);
                *result = NULL;
                goto out;
            }

            if (lookup->type != _JULE_TREE && lookup->type != _JULE_BUILTIN_FN) {
                status = JULE_ERR_NOT_A_FN;
                jule_make_not_a_fn_error(interp, value->line, lookup->type);
                *result = NULL;
                goto out;
            }

            fn          = lookup;
            values.data = value->eval_values.data + 1;
            values.len  = value->eval_values.len  - 1;
            values.cap  = 0;
invoke:;
            fn->line = value->line; /* @bad */

            status = jule_invoke(interp, value, fn, values, result);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out;
            }
            break;
        default:
            assert(0);
            break;
    }

out:;
    if (*result != NULL) {
        (*result)->line = value->line;
    }

    return status;
}



static Jule_Status jule_args(Jule_Interp *interp, Jule_Value *tree, const char *legend, Jule_Array values, ...) {
    Jule_Status   status;
    va_list       args;
    unsigned      count;
    Jule_Array    eval = JULE_ARRAY_INIT;
    unsigned      i;
    int           no_eval;
    int           deep_copy;
    int           c;
    Jule_Value   *v;
    Jule_Value  **ve_ptr;
    Jule_Value   *cpy;
    int           t;

    status = JULE_SUCCESS;

    va_start(args, values);

    count = 0;
    for (i = 0; i < strlen(legend); i += 1) {
        count += legend[i] != '-';
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

        v = jule_elem(&values, i);
        if (v == NULL) {
            status = JULE_ERR_ARITY;
            jule_make_arity_error(interp, tree->line, count, values.len, 0);
            goto out;
        }

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
                FOR_EACH(&eval, v) {
                    jule_free_value(v);
                }
                jule_free_array(&eval);
                goto out;
            }
            if (deep_copy) {
                cpy = jule_copy_force(*ve_ptr);
                jule_free_value(*ve_ptr);
                *ve_ptr = cpy;
            }
        }

        (*ve_ptr)->line = v->line;

        switch (c) {
            case '0': t = JULE_NIL;               break;
            case 'n': t = JULE_NUMBER;            break;
            case 's': t = JULE_STRING;            break;
            case '$': t = JULE_SYMBOL;            break;
            case 'l': t = JULE_LIST;              break;
            case 'o': t = JULE_OBJECT;            break;
            case '#': t = _JULE_LIST_OR_OBJECT;   break;
            case 'k': t = _JULE_STRING_OR_NUMBER; break;
            case '*': t = -1;                     break;
            default:  t = JULE_UNKNOWN;           break;
        }

        if (t == _JULE_LIST_OR_OBJECT
        &&  ((*ve_ptr)->type == JULE_LIST || (*ve_ptr)->type == JULE_OBJECT)) {

            /* Fine. */
        } else if (t == _JULE_STRING_OR_NUMBER
        &&  ((*ve_ptr)->type == JULE_STRING || (*ve_ptr)->type == JULE_NUMBER)) {

            /* Fine. */
        } else if (t >= 0 && (*ve_ptr)->type != t) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, v->line, t, (*ve_ptr)->type);
            goto out;
        }

        i += 1;

        jule_push(&eval, *ve_ptr);

        no_eval = deep_copy = 0;

nextc:;
        legend += 1;
    }

    if (i != values.len) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, count, values.len, 0);
        goto out;
    }

out:;
    jule_free_array(&eval);

    va_end(args);

    return status;
}

static Jule_Status jule_builtin_set(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "-$*", values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    jule_install_var(interp, sym->symbol, val);

    *result = sym;

out:;
    return status;
}

static Jule_Status jule_builtin_local(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *val;
    Jule_Value  *cpy;

    status = jule_args(interp, tree, "-$*", values, &sym, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (val->in_symtab) {
        cpy = jule_copy_force(val);
        jule_free_value(val);
        val = cpy;
    }

    jule_install_local(interp, sym->symbol, val);

    *result = sym;

out:;
    return status;
}

static Jule_Status jule_builtin_fn(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *def_tree;
    Jule_Value  *it;
    Jule_Value  *sym;

    status = JULE_SUCCESS;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    def_tree = jule_elem(&values, 0);

    if (def_tree->type == _JULE_TREE) {
        FOR_EACH(&def_tree->eval_values, it) {
            if (it->type != JULE_SYMBOL) {
                status = JULE_ERR_TYPE;
                jule_make_type_error(interp, def_tree->line, JULE_SYMBOL, it->type);
                *result = NULL;
                goto out;
            }
        }
        sym = jule_elem(&def_tree->eval_values, 0);
    } else if (def_tree->type == JULE_SYMBOL) {
        sym = def_tree;
    } else {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, def_tree->line, JULE_SYMBOL, def_tree->type);
        *result = NULL;
        goto out;
    }

    jule_install_var(interp, sym->symbol, jule_copy(tree));

    *result = jule_copy(sym);

out:;
    return status;
}

static Jule_Status jule_builtin_id(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *value;
    Jule_Value  *ev;
    Jule_Value  *lookup;

    status = jule_args(interp, tree, "-*", values, &value);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    switch (value->type) {
        case JULE_NUMBER:
        case JULE_STRING:
        case JULE_LIST:
        case JULE_OBJECT:
            status = jule_eval(interp, value, &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out_free;
            }
            break;
        case JULE_SYMBOL:
            lookup = jule_lookup(interp, value->symbol);
            if (lookup == NULL) {
                status = JULE_ERR_LOOKUP;
                jule_make_lookup_error(interp, value->line, value->symbol);
                *result = NULL;
                goto out_free;
            }

            ev = jule_copy(lookup);
            break;
        case _JULE_TREE:
            ev = jule_copy(value);
            break;
        default:
            assert(0);
            break;
    }

    *result = ev;

out_free:;
    jule_free_value(value);

out:;
    return status;
}


static Jule_Status jule_builtin_add(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_sub(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_mul(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_div(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_mod(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_equ(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "**", values, &a, &b);
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

static Jule_Status jule_builtin_neq(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "**", values, &a, &b);
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

static Jule_Status jule_builtin_lss(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_leq(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_gtr(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_geq(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *a;
    Jule_Value  *b;

    status = jule_args(interp, tree, "nn", values, &a, &b);
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

static Jule_Status jule_builtin_and(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;

    status = JULE_SUCCESS;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    if (cond->number == 0) {
        *result = jule_number_value(0);
        goto out_free_cond;
    }

    jule_free_value(cond);

    status = jule_eval(interp, jule_elem(&values, 1), &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    if (cond->number == 0) {
        *result = jule_number_value(0);
        goto out_free_cond;
    }

    *result = jule_number_value(1);

out_free_cond:;
    jule_free_value(cond);

out:;
    return status;
}

static Jule_Status jule_builtin_or(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;

    status = JULE_SUCCESS;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    if (cond->number == 1) {
        *result = jule_number_value(1);
        goto out_free_cond;
    }

    jule_free_value(cond);

    status = jule_eval(interp, jule_elem(&values, 1), &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }

    if (cond->number == 1) {
        *result = jule_number_value(1);
        goto out_free_cond;
    }

    *result = jule_number_value(0);

out_free_cond:;
    jule_free_value(cond);

out:;
    return status;
}

static Jule_Status _jule_builtin_print(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result, int nl) {
    Jule_Status  status;
    Jule_Value  *it;
    Jule_Value  *ev;

    (void)tree;

    FOR_EACH(&values, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
        jule_print(interp, ev, 0);

        if (it == jule_last(&values)) {
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

static Jule_Status jule_builtin_print(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    return _jule_builtin_print(interp, tree, values, result, 0);
}

static Jule_Status jule_builtin_println(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    return _jule_builtin_print(interp, tree, values, result, 1);
}

static Jule_Status jule_builtin_fmt(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *fmt;
    unsigned     n;
    char         last;
    char         c;
    unsigned     i;
    Jule_Array   args;
    Jule_Array   strings;
    Jule_Value  *it;
    Jule_Value  *ev;
    char        *s;
    int          len;
    char        *formatted;
    char        *ins;
    int          sublen;

    status = JULE_SUCCESS;

    if (values.len < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 1, values.len, 1);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &fmt);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (fmt->type != JULE_STRING) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, fmt->line, JULE_STRING, fmt->type);
        *result = NULL;
        goto out_free_fmt;
    }

    n    = 0;
    last = 0;
    for (i = 0; i < fmt->string.len; i += 1) {
        c     = fmt->string.chars[i];
        n    += (c == '%' & last != '\\');
        last  = c;
    }

    if (values.len - 1 != n) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, n + 1, values.len, 0);
        *result = NULL;
        goto out_free_fmt;
    }

    args.data = values.data + 1;
    args.len  = values.len  - 1;
    args.cap  = 0;

    memset(&strings, 0, sizeof(strings));

    FOR_EACH(&args, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free_strings;
        }
        s = jule_to_string(ev, JULE_NO_QUOTE);
        jule_push(&strings, s);
        jule_free_value(ev);
    }

    len = fmt->string.len - n;
    FOR_EACH(&strings, s) {
        len += strlen(s);
    }

    formatted = JULE_MALLOC(len);
    ins       = formatted;

    n    = 0;
    last = 0;
    for (i = 0; i < fmt->string.len; i += 1) {
        c = fmt->string.chars[i];
        if (c == '%' && last != '\\') {
            s      = jule_elem(&strings, n);
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

    *result = jule_string_value(formatted, len);
    JULE_FREE(formatted);

out_free_strings:;
    FOR_EACH(&strings, s) {
        JULE_FREE(s);
    }
    jule_free_array(&strings);

out_free_fmt:;
    jule_free_value(fmt);

out:;
    return status;
}

static Jule_Status jule_builtin_pad(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *w;
    Jule_Value  *val;
    int          width;
    int          ljust;
    char        *s;
    int          len;
    int          padding;
    char        *padded;

    status = jule_args(interp, tree, "n*", values, &w, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    width = (int)w->number;
    ljust = width < 0;
    s     = jule_to_string(val, JULE_NO_QUOTE);
    len   = strlen(s);

    if (ljust) { width = -width; }

    padding = width > len
                ? width - len
                : 0;

    padded = JULE_MALLOC(len + padding);
    memset(padded, ' ', len + padding);
    memcpy(padded + ((!ljust) * padding), s, len);

    *result = jule_string_value(padded, len + padding);

    JULE_FREE(padded);
    JULE_FREE(s);

    jule_free_value(w);
    jule_free_value(val);

out:;
    return status;
}

static Jule_Status jule_builtin_do(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *it;
    Jule_Value  *ev;

    status  = JULE_SUCCESS;
    *result = NULL;

    if (values.len < 1) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 1, values.len, 1);
        *result = NULL;
        goto out;
    }

    *result = jule_nil_value();

    FOR_EACH(&values, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            jule_free_value(*result);
            *result = NULL;
            goto out;
        }

        jule_free_value(*result);
        *result = ev;
    }

out:;
    return status;
}

static Jule_Status jule_builtin_if(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *cond;
    int          which;
    Jule_Value  *then;

    status = JULE_SUCCESS;

    if (values.len < 2 || values.len > 3) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, values.len < 2 ? 2 : 3, values.len, values.len < 2);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &cond);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (cond->type != JULE_NUMBER) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
        goto out_free_cond;
    }


    which = 1 + (cond->number == 0);

    then = jule_elem(&values, which);
    if (then != NULL) {
        status = jule_eval(interp, then, &then);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free_cond;
        }
        *result = then;
    } else {
        *result = jule_number_value(0);
    }

out_free_cond:;
    jule_free_value(cond);

out:;
    return status;
}

static Jule_Status jule_builtin_while(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *_cond;
    Jule_Value  *_expr;
    Jule_Value  *expr;
    Jule_Value  *cond;
    int          cont;

    status = JULE_SUCCESS;

    *result = NULL;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    _cond = jule_elem(&values, 0);
    _expr = jule_elem(&values, 1);
    expr  = NULL;

    for (;;) {
        status = jule_eval(interp, _cond, &cond);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }

        if (cond->type != JULE_NUMBER) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, cond->line, JULE_NUMBER, cond->type);
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

        status = jule_eval(interp, _expr, &expr);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out;
        }
    }

out:;
    return status;
}

static Jule_Status jule_builtin_repeat(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *sym;
    Jule_Value  *times;
    Jule_Value  *_expr;
    int          iters;
    int          i;
    Jule_Value  *expr;

    *result = NULL;

    status = jule_args(interp, tree, "-$n-*", values, &sym, &times, &_expr);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    iters = (int)times->number;

    for (i = 0; i < iters; i += 1) {
        jule_install_var(interp, sym->symbol, jule_number_value(i));

        status = jule_eval(interp, _expr, &expr);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free;
        }

        if (i < (iters - 1)) {
            jule_free_value(expr);
        } else {
            *result = expr;
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out_free:;
    jule_free_value(times);
    jule_free_value(sym);
    jule_free_value(_expr);

out:;
    return status;
}

static Jule_Status jule_builtin_foreach(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *sym;
    Jule_Value   *container;
    Jule_Value   *expr;
    Jule_Value   *it;
    Jule_Value   *ev;
    Jule_Value  **val;
    unsigned      i;

    *result = NULL;

    status = jule_args(interp, tree, "-$#-*", values, &sym, &container, &expr);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (container->type == JULE_LIST) {
        FOR_EACH(&container->list, it) {
            jule_install_var(interp, sym->symbol, jule_copy(it));

            status = jule_eval(interp, expr, &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out_free;
            }

            if (it == jule_last(&container->list)) {
                *result = ev;
            } else {
                jule_free_value(ev);
            }
        }
    } else {
        i = 0;
        hash_table_traverse((_Jule_Object)container->object, it, val) {
            jule_install_var(interp, sym->symbol, jule_copy(it));

            status = jule_eval(interp, expr, &ev);
            if (status != JULE_SUCCESS) {
                *result = NULL;
                goto out_free;
            }

            i += 1;

            if (i == hash_table_len((_Jule_Object)container->object)) {
                *result = ev;
            } else {
                jule_free_value(ev);
            }
        }
    }

    if (*result == NULL) {
        *result = jule_nil_value();
    }

out_free:;
    jule_free_value(sym);
    jule_free_value(container);
    jule_free_value(expr);

out:;
    return status;
}

static Jule_Status jule_builtin_list(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *list;
    Jule_Value  *it;
    Jule_Value  *ev;

    (void)tree;

    status = JULE_SUCCESS;

    list = jule_list_value();

    FOR_EACH(&values, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free;
        }
        jule_push(&list->list, ev);
    }

    *result = list;
    goto out;

out_free:;
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_dot(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *first;
    Jule_Value  *second;
    Jule_Value  *list;

    status = jule_args(interp, tree, "**", values, &first, &second);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    list = jule_list_value();

    jule_push(&list->list, first);
    jule_push(&list->list, second);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_object(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status  status;
    Jule_Value  *object;
    Jule_Value  *it;
    Jule_Value  *ev;
    Jule_Value  *key;
    Jule_Value  *val;

    (void)tree;

    status = JULE_SUCCESS;

    object = jule_object_value();

    FOR_EACH(&values, it) {
        status = jule_eval(interp, it, &ev);
        if (status != JULE_SUCCESS) {
            *result = NULL;
            goto out_free;
        }

        if (ev->type != JULE_LIST) {
            status = JULE_ERR_TYPE;
            jule_make_type_error(interp, ev->line, JULE_LIST, ev->type);
            *result = NULL;
            goto out_free;
        }

        if (ev->list.len < 2) {
            status = JULE_ERR_MISSING_VAL;
            jule_make_interp_error(interp, it->line, status);
            *result = NULL;
            goto out_free;
        }

        key = jule_elem(&ev->list, 0);
        val = jule_elem(&ev->list, 1);

        if (key->type != JULE_STRING && key->type != JULE_NUMBER) {
            status = JULE_ERR_OBJECT_KEY_TYPE;
            jule_make_object_key_type_error(interp, key->line, key->type);
            *result = NULL;
            goto out_free;
        }

        jule_insert(object, key, val);
    }

    *result = object;
    goto out;

out_free:;
    jule_free_value(object);

out:;
    return status;
}

static Jule_Status jule_builtin_in(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *key;
    Jule_Value  **lookup;

    status = JULE_SUCCESS;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &object);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (object->type != JULE_OBJECT) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, object->line, JULE_OBJECT, object->type);
        *result = NULL;
        goto out_free_object;
    }

    status = jule_eval(interp, jule_elem(&values, 1), &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free_object;
    }

    if (key->type != JULE_NUMBER && key->type != JULE_STRING) {
        status = JULE_ERR_OBJECT_KEY_TYPE;
        jule_make_object_key_type_error(interp, key->line, key->type);
        *result = NULL;
        goto out_free_key;
    }

    lookup = hash_table_get_val((_Jule_Object)object->object, key);

    *result = jule_number_value(lookup != NULL);

out_free_key:;
    jule_free_value(key);

out_free_object:;
    jule_free_value(object);

out:;
    return status;
}


static Jule_Status jule_builtin_elem(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *idx;
    unsigned      i;

    status = jule_args(interp, tree, "ln", values, &list, &idx);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    i = (int)idx->number;

    if (i >= list->list.len) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, idx->line, jule_copy(idx));
        *result = NULL;
        goto out_free;
    }

    *result = jule_copy(jule_elem(&list->list, i));

out_free:;
    jule_free_value(idx);
    jule_free_value(list);

out:;
    return status;
}

static Jule_Status jule_builtin_append(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *list;
    Jule_Value   *val;

    status = jule_args(interp, tree, "l!*", values, &list, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    jule_push(&list->list, val);

    *result = list;

out:;
    return status;
}

static Jule_Status jule_builtin_field(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *key;
    Jule_Value  **lookup;

    status = JULE_SUCCESS;

    if (values.len != 2) {
        status = JULE_ERR_ARITY;
        jule_make_arity_error(interp, tree->line, 2, values.len, 0);
        *result = NULL;
        goto out;
    }

    status = jule_eval(interp, jule_elem(&values, 0), &object);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    if (object->type != JULE_OBJECT) {
        status = JULE_ERR_TYPE;
        jule_make_type_error(interp, object->line, JULE_OBJECT, object->type);
        *result = NULL;
        goto out_free_object;
    }

    status = jule_eval(interp, jule_elem(&values, 1), &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out_free_object;
    }

    if (key->type != JULE_NUMBER && key->type != JULE_STRING) {
        status = JULE_ERR_OBJECT_KEY_TYPE;
        jule_make_object_key_type_error(interp, key->line, key->type);
        *result = NULL;
        goto out_free_key;
    }

    lookup = hash_table_get_val((_Jule_Object)object->object, key);

    if (lookup == NULL) {
        status = JULE_ERR_BAD_INDEX;
        jule_make_bad_index_error(interp, key->line, jule_copy(key));
        *result = NULL;
        goto out_free_key;
    } else {
        *result = jule_copy(*lookup);
    }

out_free_key:;
    jule_free_value(key);

out_free_object:;
    jule_free_value(object);

out:;
    return status;
}

static Jule_Status jule_builtin_insert(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *key;
    Jule_Value   *val;

    status = jule_args(interp, tree, "o!k!*", values, &object, &key, &val);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    jule_insert(object, key, val);

    *result = object;

out:;
    return status;
}

static Jule_Status jule_builtin_delete(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Status   status;
    Jule_Value   *object;
    Jule_Value   *key;

    status = jule_args(interp, tree, "ok", values, &object, &key);
    if (status != JULE_SUCCESS) {
        *result = NULL;
        goto out;
    }

    jule_delete(object, key);

    jule_free_value(key);

    *result = object;

out:;
    return status;
}

static Jule_Status jule_builtin_len(Jule_Interp *interp, Jule_Value *tree, Jule_Array values, Jule_Value **result) {
    Jule_Array   eval;
    Jule_Status  status;
    Jule_Value  *ev;

    memset(&eval, 0, sizeof(eval));

    status = jule_args(interp, tree, "*", values, &ev);
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
            *result = jule_number_value(ev->string.len);
            break;
        case JULE_SYMBOL:
            *result = jule_number_value(strlen(ev->symbol));
            break;
        case JULE_LIST:
            *result = jule_number_value(ev->list.len);
            break;
        case JULE_OBJECT:
            *result = jule_number_value(hash_table_len((_Jule_Object)ev->object));
            break;
        case _JULE_TREE:
            *result = jule_number_value(ev->eval_values.len);
            break;
        case _JULE_BUILTIN_FN:
            *result = jule_number_value(0);
            break;
        default:
            assert(0);
            break;
    }

    jule_free_value(ev);

out:;
    return status;
}

Jule_Status jule_init_interp(Jule_Interp *interp) {
    memset(interp, 0, sizeof(*interp));

    interp->symtab       = hash_table_make_e(Char_Ptr, Jule_Value_Ptr, jule_charptr_hash, (void*)jule_charptr_equ);
    interp->local_symtab = hash_table_make_e(Char_Ptr, Jule_Value_Ptr, jule_charptr_hash, (void*)jule_charptr_equ);

    jule_install_fn(interp, "set",     jule_builtin_set);
    jule_install_fn(interp, "local",   jule_builtin_local);
    jule_install_fn(interp, "fn",      jule_builtin_fn);
    jule_install_fn(interp, "id",      jule_builtin_id);
    jule_install_fn(interp, "+",       jule_builtin_add);
    jule_install_fn(interp, "-",       jule_builtin_sub);
    jule_install_fn(interp, "*",       jule_builtin_mul);
    jule_install_fn(interp, "/",       jule_builtin_div);
    jule_install_fn(interp, "%",       jule_builtin_mod);
    jule_install_fn(interp, "==",      jule_builtin_equ);
    jule_install_fn(interp, "!=",      jule_builtin_neq);
    jule_install_fn(interp, "<",       jule_builtin_lss);
    jule_install_fn(interp, "<=",      jule_builtin_leq);
    jule_install_fn(interp, ">",       jule_builtin_gtr);
    jule_install_fn(interp, ">=",      jule_builtin_geq);
    jule_install_fn(interp, "and",     jule_builtin_and);
    jule_install_fn(interp, "or",      jule_builtin_or);
    jule_install_fn(interp, "print",   jule_builtin_print);
    jule_install_fn(interp, "println", jule_builtin_println);
    jule_install_fn(interp, "fmt",     jule_builtin_fmt);
    jule_install_fn(interp, "pad",     jule_builtin_pad);
    jule_install_fn(interp, "do",      jule_builtin_do);
    jule_install_fn(interp, "if",      jule_builtin_if);
    jule_install_fn(interp, "while",   jule_builtin_while);
    jule_install_fn(interp, "repeat",  jule_builtin_repeat);
    jule_install_fn(interp, "foreach", jule_builtin_foreach);
    jule_install_fn(interp, "list",    jule_builtin_list);
    jule_install_fn(interp, ".",       jule_builtin_dot);
    jule_install_fn(interp, "elem",    jule_builtin_elem);
    jule_install_fn(interp, "append",  jule_builtin_append);
    jule_install_fn(interp, "object",  jule_builtin_object);
    jule_install_fn(interp, "in",      jule_builtin_in);
    jule_install_fn(interp, "field",   jule_builtin_field);
    jule_install_fn(interp, "insert",  jule_builtin_insert);
    jule_install_fn(interp, "delete",  jule_builtin_delete);
    jule_install_fn(interp, "len",     jule_builtin_len);

    return JULE_SUCCESS;
}

Jule_Status jule_interp(Jule_Interp *interp) {
    Jule_Status  status;
    Jule_Value  *root;
    Jule_Value  *result;

    status = JULE_SUCCESS;

    if (interp->roots.len == 0) {
        return JULE_ERR_NO_INPUT;
    }

    FOR_EACH(&interp->roots, root) {
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
    Jule_Value  *it;
    char        *key;
    char        *real_key;
    Jule_Value **val;

    FOR_EACH(&interp->roots, it) {
        jule_free_value(it);
    }
    jule_free_array(&interp->roots);

    hash_table_traverse(interp->symtab, key, val) {
        real_key = *hash_table_get_key(interp->symtab, key);
        JULE_FREE(real_key);
        (*val)->in_symtab = 0;
        jule_free_value(*val);
    }
    hash_table_free(interp->symtab);

    memset(interp, 0, sizeof(*interp));
}

#undef IND_LEVEL
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
