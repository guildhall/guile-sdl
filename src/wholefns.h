/* wholefns.h --- various macros for declaring/defining whole functions */

#ifndef GUILE_SDL_WHOLEFNS_H
#define GUILE_SDL_WHOLEFNS_H

#include "argcheck.h"
#include "sdlsmobs.h"
#include "retval.h"


/* number getter and setter */

#define GSDL_NUMBER_GETTER(s_func, c_func, tag, c_type, c_field)        \
GH_DEFPROC (c_func, s_func, 1, 0, 0, (SCM obj),                         \
            "Get @code{" #c_field "} from the\n"                        \
            "@code{" #c_type "} @var{obj}.")                            \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, tag, 1);                                            \
  RETURN_INT (SMOBFIELD (c_type, c_field));                             \
}

#define GSDL_NUMBER_SETTER(s_func, c_func, tag, c_type, c_field, conv)  \
GH_DEFPROC (c_func, s_func, 2, 0, 0, (SCM obj, SCM value),              \
            "Set @code{" #c_field "} in the\n"                          \
            "@code{" #c_type "} @var{obj}\n"                            \
            "to @var{value}.")                                          \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, tag, 1);                                            \
  ASSERT_EXACT (value, 2);                                              \
  SMOBFIELD (c_type, c_field) = conv (value);                           \
  RETURN_UNSPECIFIED;                                                   \
}


/* enum getter and setter */

#define GSDL_ENUM_GETTER(s_func, c_func, tag, c_type, c_field, etype)   \
GH_DEFPROC (c_func, s_func, 1, 0, 0, (SCM obj),                         \
            "")                                                         \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, tag, 1);                                            \
  return gsdl_long2enum (SMOBFIELD (c_type, c_field), etype);           \
}

#define GSDL_ENUM_SETTER(s_func, c_func, tag, c_type, c_field, etype)   \
GH_DEFPROC (c_func, s_func, 2, 0, 0, (SCM obj, SCM value),              \
            "")                                                         \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, tag, 1);                                            \
  SMOBFIELD (c_type, c_field) = GSDL_ENUM2LONG (value, etype, 1);       \
  RETURN_UNSPECIFIED;                                                   \
}


/* flag getter and setter*/

#define GSDL_FLAG_GETTER(s_func, c_func, c_tag, c_type, c_field, stash) \
GH_DEFPROC (c_func, s_func, 1, 0, 0, (SCM obj),                         \
            "")                                                         \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, c_tag, 1);                                          \
  return gsdl_ulong2flags (SMOBFIELD (c_type, c_field), stash);         \
}

#define GSDL_FLAG_SETTER(s_func, c_func, c_tag, c_type, c_field, stash) \
GH_DEFPROC (c_func, s_func, 2, 0, 0, (SCM obj, SCM value),              \
            "")                                                         \
{                                                                       \
  const char *FUNC_NAME = s_ ## c_func;                                 \
  ASSERT_SMOB (obj, c_tag, 1);                                          \
  SMOBFIELD (c_type, c_field)                                           \
    = GSDL_FLAGS2ULONG (value, stash, 2);                               \
  RETURN_UNSPECIFIED;                                                   \
}

#endif /* ! GUILE_SDL_WHOLEFNS_H */

/* wholefns.h ends here */
