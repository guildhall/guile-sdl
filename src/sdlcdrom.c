/* sdlcdrom.c --- SDL CDROM functions for Guile
 *
 * 	Copyright (C) 2003 Thien-Thi Nguyen
 * 	Copyright (C) 2001 Alex Shinn
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* config */

#include <guile/gh.h>
#include <SDL/SDL.h>

#include "config.h"
#include "argcheck.h"
#include "sdlsmobs.h"


static long cdrom_tag;

#define ASSERT_CDROM(obj,which) \
  ASSERT_SMOB (obj, cdrom_tag, which)


MDEFLOCEXP (sdl_cd_p, "sdl-cd?", 1, 0, 0,
            (SCM obj),
            "Return #t iff @var{obj} is a cd smob.")
#define FUNC_NAME s_sdl_cd_p
{
  return (SCM_SMOB_PREDICATE (cdrom_tag, obj)
          ? SCM_BOOL_T
          : SCM_BOOL_F);
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_null_p, "sdl-cd-null?", 1, 0, 0,
            (SCM cd_smob),
            "Return #t iff @var{cd} is a null pointer.")
#define FUNC_NAME s_sdl_cd_null_p
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);

  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd == NULL)
    return SCM_BOOL_T;
  else
    return SCM_BOOL_F;
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_num_drives, "sdl-cd-num-drives", 0, 0, 0,
            (),
            "Return the number of CD drives.")
#define FUNC_NAME s_sdl_cd_num_drives
{
  return (gh_long2scm (SDL_CDNumDrives ()));
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_name, "sdl-cd-name", 0, 1, 0,
            (SCM s_drive),
            "Return a human-readable, system-dependent\n"
            "identifier for the CD-ROM.\n"
            "Optional arg @var{drive} is a number specifying which drive.")
#define FUNC_NAME s_sdl_cd_name
{
  const char *name;
  int drive=0;

  if (! SCM_UNBNDP (s_drive)) {
    ASSERT_EXACT (s_drive, SCM_ARG1);
    drive = gh_scm2int (s_drive);
  }

  name = SDL_CDName (drive);
  return gh_str02scm (name);
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_open, "sdl-cd-open", 0, 1, 0,
            (SCM drive),
            "Open a CD-ROM drive for access and return its handle.\n"
            "If the drive is unavailable, return #f.\n"
            "Optional arg @var{drive} is a number specifying which drive.")
#define FUNC_NAME s_sdl_cd_open

{
  SDL_CD *cd;
  int cdrive=0;

  if (! SCM_UNBNDP (drive)) {
    ASSERT_EXACT (drive, SCM_ARG1);
    cdrive = gh_scm2int (drive);
  }

  cd = SDL_CDOpen (cdrive);
  if (! cd)
    return SCM_BOOL_F;
  else
    SCM_RETURN_NEWSMOB (cdrom_tag, cd);
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_status, "sdl-cd-status", 1, 0, 0,
            (SCM cd_smob),
            "Return the current status of the drive @var{cd}.")
#define FUNC_NAME s_sdl_cd_status
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return gh_long2scm (SDL_CDStatus (cd));
  }
  else {
    return gh_long2scm (CD_ERROR);
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_in_drive_p, "sdl-cd-in-drive?", 1, 0, 0,
            (SCM cd_smob),
            "Return #t iff there is a CD in drive @var{cd}.")
#define FUNC_NAME s_sdl_cd_in_drive_p
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    if (CD_INDRIVE (SDL_CDStatus (cd)))
      return SCM_BOOL_T;
    else
      return SCM_BOOL_F;
  }
  else {
    return SCM_BOOL_F;
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_get_num_tracks, "sdl-cd-get-num-tracks", 1, 0, 0,
            (SCM cd_smob),
            "Return the number of tracks on the @var{cd}.")
#define FUNC_NAME s_sdl_cd_get_num_tracks
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return (gh_long2scm (cd->numtracks));
  }
  else {
    return (gh_long2scm (-1));
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_get_cur_track, "sdl-cd-get-cur-track", 1, 0, 0,
            (SCM cd_smob),
            "Return the current track on the @var{cd}.")
#define FUNC_NAME s_sdl_cd_get_cur_track
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return (gh_long2scm (cd->cur_track));
  }
  else {
    return (gh_long2scm (-1));
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_get_cur_frame, "sdl-cd-get-cur-frame", 1, 0, 0,
            (SCM cd_smob),
            "Return the current frame of the @var{cd}.")
#define FUNC_NAME s_sdl_cd_get_cur_frame
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return (gh_long2scm (cd->cur_frame));
  }
  else {
    return (gh_long2scm (-1));
  }
}
#undef FUNC_NAME

SCM_SYMBOL (gsdl_sym_offset, "offset");
SCM_SYMBOL (gsdl_sym_length, "length");
SCM_SYMBOL (gsdl_sym_type, "type");
SCM_SYMBOL (gsdl_sym_id, "id");

MDEFLOCEXP (sdl_cd_get_nth_track, "sdl-cd-get-nth-track", 1, 1, 0,
            (SCM cd_smob,
             SCM s_n),
            "Return info for @var{cd} track @var{n} as an alist.")
#define FUNC_NAME s_sdl_cd_get_nth_track
{
  SDL_CD *cd;
  int n=0;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (! SCM_UNBNDP (s_n)) {
    ASSERT_EXACT (s_n, SCM_ARG2);
    n = gh_scm2ulong (s_n);
  }

  if ((cd != NULL) && (n < cd->numtracks)) {
    /* Form an assoc list */
    return SCM_LIST4
      (gh_cons (gsdl_sym_id,     gh_long2scm (cd->track[n].id)),
       gh_cons (gsdl_sym_type,   gh_long2scm (cd->track[n].type)),
       gh_cons (gsdl_sym_length, gh_ulong2scm (cd->track[n].length)),
       gh_cons (gsdl_sym_offset, gh_ulong2scm (cd->track[n].offset)));
  }
  else {
    return SCM_EOL;
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_play_tracks, "sdl-cd-play-tracks", 1, 4, 0,
            (SCM cd_smob,
             SCM s_start_track,
             SCM s_start_frame,
             SCM s_n_tracks,
             SCM s_n_frames),
            "Play the given CD tracks."
            "Play the @var{cd} starting at @var{start-track} and\n"
            "@var{start-frame} for @var{ntracks} tracks and @var{nframes}\n"
            "frames.  If both @var{ntrack} and @var{nframe} are 0, play\n"
            "until the end of the CD.  This procedure will skip data\n"
            "tracks, and should only be called after calling\n"
            "@code{sdl-cd-status} to get track information about the CD.")
#define FUNC_NAME s_sdl_cd_play_tracks
{
  SDL_CD *cd;
  int start_track=0, start_frame=0, n_tracks=1, n_frames=1;
  int ret;

  ASSERT_CDROM (cd_smob, SCM_ARG1);

  if (! SCM_UNBNDP (s_start_track)) {
    ASSERT_EXACT (s_start_track, SCM_ARG2);
    start_track = gh_scm2ulong (s_start_track);
  }

  if (! SCM_UNBNDP (s_start_frame)) {
    ASSERT_EXACT (s_start_frame, SCM_ARG3);
    start_frame = gh_scm2ulong (s_start_frame);
  }

  if (! SCM_UNBNDP (s_n_tracks)) {
    ASSERT_EXACT (s_n_tracks, SCM_ARG4);
    n_tracks = gh_scm2ulong (s_n_tracks);;
  }

  if (! SCM_UNBNDP (s_n_frames)) {
    ASSERT_EXACT (s_n_frames, SCM_ARG5);
    n_frames = gh_scm2ulong (s_n_frames);
  } else {
    cd = SMOBGET (cd_smob, SDL_CD *);
    n_frames = cd->track[start_track + n_tracks - 1].length;
  }

  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    ret = SDL_CDPlayTracks (cd, start_track, start_frame, n_tracks, n_frames);
  }
  else {
    ret = -1;
  }

  return gh_long2scm (ret);
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_play, "sdl-cd-play", 3, 0, 0,
            (SCM cd_smob,
             SCM s_start,
             SCM s_length),
            "Play a @var{cd} from @var{start} frame for\n"
            "@var{length} frames.")
#define FUNC_NAME s_sdl_cd_play
{
  SDL_CD *cd;
  int ret;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  ASSERT_EXACT (s_start, SCM_ARG2);
  ASSERT_EXACT (s_length, SCM_ARG3);

  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    ret = SDL_CDPlay (cd,
                      gh_scm2ulong (s_start),
                      gh_scm2ulong (s_length));
  }
  else {
    ret = -1;
  }

  return gh_long2scm (ret);
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_pause, "sdl-cd-pause", 1, 0, 0,
            (SCM cd_smob),
            "Pause a @var{cd}.")
#define FUNC_NAME s_sdl_cd_pause
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return gh_long2scm (SDL_CDPause (cd));
  }
  else {
    return gh_long2scm (-1);
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_resume, "sdl-cd-resume", 1, 0, 0,
            (SCM cd_smob),
            "Resume (unpause) a @var{cd}.")
#define FUNC_NAME s_sdl_cd_resume
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return gh_long2scm (SDL_CDResume (cd));
  }
  else {
    return gh_long2scm (-1);
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_stop, "sdl-cd-stop", 1, 0, 0,
            (SCM cd_smob),
            "Stop a @var{cd}.")
#define FUNC_NAME s_sdl_cd_stop
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return gh_long2scm (SDL_CDStop (cd));
  }
  else {
    return gh_long2scm (-1);
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_eject, "sdl-cd-eject", 1, 0, 0,
            (SCM cd_smob),
            "Eject a @var{cd}.")
#define FUNC_NAME s_sdl_cd_eject
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    return gh_long2scm (SDL_CDEject (cd));
  }
  else {
    return gh_long2scm (-1);
  }
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_close, "sdl-cd-close", 1, 0, 0,
            (SCM cd_smob),
            "Close a @var{cd}.")
#define FUNC_NAME s_sdl_cd_close
{
  SDL_CD *cd;

  ASSERT_CDROM (cd_smob, SCM_ARG1);
  cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) {
    SDL_CDClose (cd);
    SMOBSET (cd_smob, NULL);
  }

  return SCM_UNSPECIFIED;
}
#undef FUNC_NAME


MDEFLOCEXP (sdl_cd_msf_to_frames, "sdl-cd-msf->frames", 1, 2, 0,
            (SCM s_m,
             SCM s_s,
             SCM s_f),
            "Convert minute @var{m}, second @var{s}, frame @var{f}\n"
            "to frames.  @var{s} and @var{f} are optional.")
#define FUNC_NAME s_sdl_cd_msf_to_frames
{
  int frames;
  int m, s=0, f=0;

  ASSERT_EXACT (s_m, SCM_ARG1);
  m = gh_scm2ulong (s_m);

  if (! SCM_UNBNDP (s_s)) {
    ASSERT_EXACT (s_s, SCM_ARG2);
    s = gh_scm2ulong (s_s);
  }

  if (! SCM_UNBNDP (s_f)) {
    ASSERT_EXACT (s_f, SCM_ARG3);
    f = gh_scm2ulong (s_f);
  }

  frames = MSF_TO_FRAMES (m, s, f);
  return gh_long2scm (frames);
}
#undef FUNC_NAME


SCM_SYMBOL (gsdl_sym_f, "f");
SCM_SYMBOL (gsdl_sym_s, "s");
SCM_SYMBOL (gsdl_sym_m, "m");

MDEFLOCEXP (sdl_cd_frames_to_msf, "sdl-cd-frames->msf", 1, 0, 0,
            (SCM s_frames),
            "Return a minute/second/frames alist made from\n"
            "converting @var{frames} (a number).")
#define FUNC_NAME s_sdl_cd_frames_to_msf
{
  int frames, m, s, f;

  ASSERT_EXACT (s_frames, SCM_ARG1);
  frames = gh_scm2ulong (s_frames);

  FRAMES_TO_MSF (frames, &m , &s, &f);
  return SCM_LIST3 (gh_cons (gsdl_sym_m, gh_ulong2scm (m)),
                    gh_cons (gsdl_sym_s, gh_ulong2scm (s)),
                    gh_cons (gsdl_sym_f, gh_ulong2scm (f)));
}
#undef FUNC_NAME

/*-------------------------------------------------------------*/

static
SCM
mark_cd (SCM cd_smob)
{
  return cd_smob;
}

static
size_t
free_cd (SCM cd_smob)
{
  SDL_CD *cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL)
    SDL_CDClose (cd);

  return 0;
}

static
int
print_cd (SCM cd_smob, SCM port, scm_print_state *pstate)
{
  SDL_CD *cd = SMOBGET (cd_smob, SDL_CD *);

  if (cd != NULL) { /* Print the current status */

    scm_puts ("#<SDL-CD ", port);

    switch (cd->status) {

    case CD_TRAYEMPTY:
      scm_puts (" [TRAY EMPTY]>", port);
      break;

    case CD_STOPPED:
      scm_puts (" [STOPPED]>", port);
      break;

    case CD_PLAYING:
      scm_puts (" [PLAYING]>", port);
      break;

    case CD_PAUSED:
      scm_puts (" [PAUSED]>", port);
      break;

    case CD_ERROR:
      scm_puts (" [DRIVE ERROR]>", port);
      break;

    }
  }
  else {
    scm_puts ("#<SDL-CD NULL>", port);
  }

  /* Non-zero means success */
  return 1;
}


void
gsdl_init_cdrom (void)
{
  cdrom_tag = scm_make_smob_type ("SDL-CD", sizeof (SDL_CD));
  scm_set_smob_mark  (cdrom_tag, mark_cd);
  scm_set_smob_free  (cdrom_tag, free_cd);
  scm_set_smob_print (cdrom_tag, print_cd);

#include "sdlcdrom.x"
}

/* sdlcdrom.c ends here */
