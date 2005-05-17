;;; simple.scm --- high abstractions for quick experimentation

;;	Copyright (C) 2005 Thien-Thi Nguyen
;;
;; This is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.
;;
;; This software is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this package; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;; Boston, MA  02110-1301  USA

;;; Code:

(define-module (sdl simple)
  #:use-module ((sdl sdl) #:renamer (symbol-prefix-proc '|||-))
  #:use-module ((sdl ttf) #:renamer (symbol-prefix-proc '|T|-))
  #:use-module ((sdl gfx) #:renamer (symbol-prefix-proc '|G|-))
  #:export (simple-canvas
            simple-stylus
            simple-vpacked-image))

;; Return a @dfn{canvas closure} that accepts a few simple messages.
;; If @var{init?} is non-#f, initalize the SDL video subsystem first.
;; @var{w}, @var{h}, and @var{bpp} specify the width, height, and
;; bits-per-pixel, respectively.
;; @var{flags} are symbols to set the video mode.  If omitted, the
;; default is @code{SDL_HWSURFACE} and @code{SDL_DOUBLEBUF}.
;;
;; The closure, if called without arguments, returns the video surface.
;; Otherwise, the following messages are recognized:
;;
;; @table @code
;; @item #:rect
;; Return a rectangle the width and height of the canvas.
;;
;; @item #:set-bg! r g b
;; Set the background color (used for clearing) to the color specified
;; by @var{r}, @var{g} and @var{b} (integers 0-255), respectively.
;; By default it is black (all values zero).
;;
;; @item #:clear!
;; Fill the canvas with the background color.
;;
;; @item #:w
;; @itemx #:h
;; @itemx #:w/h
;; Return width, height, or a cons of width and height, respectively.
;; @end table
;;
(define (simple-canvas init? w h bpp . flags)
  (or (not init?)
      (= 0 (|||-init '(SDL_INIT_VIDEO)))
      (error "could not init SDL"))
  (let ((canvas (|||-set-video-mode
                    w h bpp (if (null? flags)
                               '(SDL_HWSURFACE SDL_DOUBLEBUF)
                               flags)))
        (rect (|||-make-rect 0 0 w h))
        (bg #f))
    (define (set-bg! r g b)
      (set! bg (|||-map-rgb (assq-ref (|||-get-video-info) 'vfmt) r g b)))
    (set-bg! 0 0 0)
    ;; rv
    (lambda args
      (if (null? args)
          canvas
          (case (car args)
            ((#:rect) rect)
            ((#:set-bg!) (apply set-bg! (cdr args)))
            ((#:clear!) (|||-fill-rect canvas rect bg))
            ((#:w) w)
            ((#:h) h)
            ((#:w/h) (cons w h))
            (else (error "bad key:" (car args))))))))

;; Return a @dfn{stylus closure} that accepts a few simple messages.
;; If @var{init?} is non-#f, initialize the SDL True Type support first.
;; @var{filename} specifes the .ttf file to load and @code{size} the size.
;; @var{r}, @var{g} and @var{b} are integers (0-255) specifying the color.
;; The closure recognizes the following messages:
;;
;; @table @code
;; @item #:set-font! filename size
;; @itemx #:set-color! r g b
;; Change the font or color, respectively.
;;
;; @item #:set-canvas! surface
;; Set the surface on which the @code{#:write!} command renders.
;;
;; @item #:render text
;; Return a surface of @var{text} rendered using the default
;; font, size, color and size.
;;
;; @item #:write! text x y
;; Similar to #:render, but also blit the surface onto the canvas
;; at the position specified by @var{x} and @var{y}.
;; @end table
;;
(define (simple-stylus init? filename size r g b)
  (or (not init?)
      (|T|-ttf-init)
      (error "could not init font lib"))
  (let ((font #f) (color #f) (canvas #f))
    (define (set-font! filename size)
      (set! font (|T|-load-font filename size)))
    (define (set-color! r g b)
      (set! color (|||-make-color r g b)))
    (define (set-canvas! v)
      (set! canvas v))
    (define (render text)
      (|T|-render-text font text color))
    (set-font! filename size)
    (set-color! r g b)
    ;; rv
    (lambda (key . args)
      (case key
        ((#:set-font!) (apply set-font! (cdr args)))
        ((#:set-color!) (apply set-color! (cdr args)))
        ((#:set-canvas!) (set-canvas! (cadr args)))
        ((#:render) (render (car args)))
        ((#:write!) (|||-blit-surface (render (car args)) #f canvas))
        (else (error "bad key:" key))))))

;; Return a @dfn{vpacked image closure} that accepts a few simple messages.
;; "Vpacked" means multiple vertically-abutted images of dimensions NxN (at
;; the top) through Nx1 (at the bottom), stored in a single image file.
;; @var{filename} specifies the file and optional arg @var{canvas} specifies
;; a surface for blitting.  The closure recognizes the following messages:
;;
;; @table @code
;; @item #:set-canvas! surface
;; Change the canvas.
;;
;; @item #:rects
;; Return the vector of rectangles of length N+1 (the element at index
;; zero is #f) corresponding to areas on the image representing the smaller
;; sub-images.  The element at index I is a rectangle of dimension IxI.
;;
;; @item #:blit! i rect
;; Blit the sub-image @var{i} (an integer 1 <= I <= N), onto the canvas.
;; @var{rect} specifies a rectangle to blit to.
;; @end table
;;
;;-sig: (filename [canvas])
;;
(define (simple-vpacked-image filename . etc)
  (let* ((image (|||-load-image filename))
         (rects (let loop ((size (|||-surface:w image)) (offset 0) (acc '()))
                  (if (= 0 size)
                      (list->vector (cons #f acc))
                      (loop (1- size) (+ offset size)
                            (cons (|||-make-rect 0 offset size size) acc)))))
         (canvas (and (not (null? etc)) (car etc))))
    (define (set-canvas! v)
      (set! canvas v))
    ;; rv
    (lambda (key . args)
      (case key
        ((#:set-canvas!) (set-canvas! (car args)))
        ((#:rects) rects)
        ((#:blit!) (|||-blit-surface
                       image (vector-ref rects (car args))
                       canvas (cadr args)))
        (else (error "bad key:" key))))))

;;; simple.scm ends here
