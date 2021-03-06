/* sdl.c --- SDL Wrappers for Guile
 *
 * Copyright (C) 2003, 2004, 2005, 2007, 2008, 2009, 2011 Thien-Thi Nguyen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "guile-sdl.h"

struct obtw *btw;


/* Forward declarations on this page.  */

#define DECLARE_INIT_THUNK(x)  GBO void gsdl_init_ ## x (void)

DECLARE_INIT_THUNK (enums);
DECLARE_INIT_THUNK (rect);
DECLARE_INIT_THUNK (color);
DECLARE_INIT_THUNK (video);
DECLARE_INIT_THUNK (surface);
DECLARE_INIT_THUNK (event);
DECLARE_INIT_THUNK (joystick);
DECLARE_INIT_THUNK (cdrom);
DECLARE_INIT_THUNK (misc);


static SCM init_flags;

PRIMPROC
(get_init_flags, "flagstash:init", 0, 0, 0, (),
 doc: /***********
Return the flagstash object for @code{init} flags.
@xref{Enums and Constants}.  */)
{
  return init_flags;
}


/* Initialization */
PRIMPROC
(init, "init", 1, 0, 0,
 (SCM sel),
 doc: /***********
Initialize SDL based on configuration flags @var{sel}.
@var{sel} is a list of symbols whose names all begin
with @code{SDL_INIT_}.  */)
{
#define FUNC_NAME s_init
  RETURN_INT (SDL_Init (GSDL_FLAGS2ULONG (sel, init_flags, 1)));
#undef FUNC_NAME
}


PRIMPROC
(init_subsystem, "init-subsystem", 1, 0, 0,
 (SCM sel),
 doc: /***********
Initialize the SDL subsystems represented by @var{sel}.
@var{sel} is a list of flags (symbols)
from the same set useful for @code{init}.  */)
{
#define FUNC_NAME s_init_subsystem
  RETURN_INT (SDL_InitSubSystem (GSDL_FLAGS2ULONG (sel, init_flags, 1)));
#undef FUNC_NAME
}


/* Termination */
PRIMPROC
(quit, "quit", 0, 0, 0,
 (void),
 doc: /***********
Shut down all SDL subsystems.
Return @code{#t}.  */)
{
#define FUNC_NAME s_quit
  scm_gc ();
  SDL_Quit ();
  RETURN_TRUE;
#undef FUNC_NAME
}


PRIMPROC
(quit_subsystem, "quit-subsystem", 1, 0, 0,
 (SCM sel),
 doc: /***********
Shut down the SDL subsystems represented by @var{sel}.
@var{sel} is a list of flags (symbols)
from the same set useful for @code{init}.
Return @code{#t}.  */)
{
#define FUNC_NAME s_quit_subsystem
  scm_gc ();
  SDL_QuitSubSystem (GSDL_FLAGS2ULONG (sel, init_flags, 1));
  RETURN_TRUE;
#undef FUNC_NAME
}


/* Information */
PRIMPROC
(was_init, "was-init", 1, 0, 0,
 (SCM sel),
 doc: /***********
Check if the SDL subsystems represented by @var{sel} have
been initialized.  @var{sel} is a list of flags (symbols)
from the same set useful for @code{init}.  Return a list
likewise composed.  */)
{
#define FUNC_NAME s_was_init
  return btw->ulong2flags (SDL_WasInit (GSDL_FLAGS2ULONG
                                        (sel, init_flags, 1)),
                           init_flags);
#undef FUNC_NAME
}


/* time functions */

PRIMPROC
(get_ticks, "get-ticks", 0, 0, 0,
 (void),
 doc: /***********
Return the number of milliseconds since
the SDL library initialization.  */)
{
#define FUNC_NAME s_get_ticks
  RETURN_INT (SDL_GetTicks ());
#undef FUNC_NAME
}


PRIMPROC
(delay, "delay", 1, 0, 0,
 (SCM ms),
 doc: /***********
Wait @var{ms} milliseconds.  */)
{
#define FUNC_NAME s_delay
  ASSERT_INTEGER (ms, 1);
  SDL_Delay (C_ULONG (ms));
  RETURN_UNSPECIFIED;
#undef FUNC_NAME
}


/* error handling */

PRIMPROC
(get_error, "get-error", 0, 0, 0,
 (void),
 doc: /***********
Return the current SDL error string.  */)
{
#define FUNC_NAME s_get_error
  char *error = SDL_GetError ();
  RETURN_0STR (error);
#undef FUNC_NAME
}


PRIMPROC
(obtw, "%%Guile-SDL-obtw", 0, 0, 0,
 (void),
 doc: /***********
Internal procedure; do not use.  */)
{
#define FUNC_NAME s_obtw
  return PACK_POINTER (btw);
#undef FUNC_NAME
}

#include "init.c"

static
void
init_module (void)
{
  /* Oh, by the way...  */
  btw = calloc (1, sizeof (*btw));

  /* Initialize enums first, so we can use them.  */
  gsdl_init_enums ();

  /* Init flags.  */
  init_flags = btw->make_flagstash (&init_flagstash);

#include "sdl.x"

  /* Initialize subsystems.  */
  gsdl_init_rect ();
  gsdl_init_color ();
  gsdl_init_video ();
  gsdl_init_surface ();
  gsdl_init_event ();
  gsdl_init_joystick ();
  gsdl_init_cdrom ();
  gsdl_init_misc ();
}

MOD_INIT_LINK_THUNK ("sdl sdl", sdl_sdl, init_module)

/* sdl.c ends here */
