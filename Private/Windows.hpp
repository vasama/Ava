#pragma once

#include "Ava/Misc.hpp"

#if !Ava_WINDOWS
#	error Include on Windows only
#endif

#ifndef UNICODE
#	define UNICODE
#endif

#ifndef Ava_WIN32_FAT_AND_FRIENDLY
#	define WIN32_LEAN_AND_MEAN
#endif

// Atom Manager routines
#ifndef Ava_WIN32_ATOM
#	define NOATOM
#endif

// Clipboard routines
#ifndef Ava_WIN32_CLIPBOARD
#	define NOCLIPBOARD
#endif

// Screen colors
#ifndef Ava_WIN32_COLOR
#	define NOCOLOR
#endif

// COMM driver routines
#ifndef Ava_WIN32_COMM
#	define NOCOMM
#endif

// Control and Dialog routines
#ifndef Ava_WIN32_CTLMGR
#	define NOCTLMGR
#endif

// DeferWindowPos routines
#ifndef Ava_WIN32_DEFERWINDOWPOS
#	define NODEFERWINDOWPOS
#endif

// DrawText() and DT_*
#ifndef Ava_WIN32_DRAWTEXT
#	define NODRAWTEXT
#endif

// All GDI defines and routines
#ifndef Ava_WIN32_GDI
#	define NOGDI
#endif

// CC_* , LC_*, PC_*, CP_*, TC_*, RC_
#ifndef Ava_WIN32_GDICAPMASKS
#	define NOGDICAPMASKS
#endif

// Help engine interface.
#ifndef Ava_WIN32_HELP
#	define NOHELP
#endif

// IDI_*
#ifndef Ava_WIN32_ICONS
#	define NOICONS
#endif

// Kanji support stuff.
#ifndef Ava_WIN32_KANJI
#	define NOKANJI
#endif

// All KERNEL defines and routines
#ifndef Ava_WIN32_KERNEL
#	define NOKERNEL
#endif

// MK_*
#ifndef Ava_WIN32_KEYSTATES
#	define NOKEYSTATES
#endif

// MB_* and MessageBox()
#ifndef Ava_WIN32_MB
#	define NOMB
#endif

// Modem Configuration Extensions
#ifndef Ava_WIN32_MCX
#	define NOMCX
#endif

// GMEM_* , LMEM_*, GHND, LHND, associated routines
#ifndef Ava_WIN32_MEMMGR
#	define NOMEMMGR
#endif

// MF_*
#ifndef Ava_WIN32_MENUS
#	define NOMENUS
#endif

// typedef METAFILEPICT
#ifndef Ava_WIN32_METAFILE
#	define NOMETAFILE
#endif

// typedef MSG and associated routines
#ifndef Ava_WIN32_MSG
#	define NOMSG
#endif

// All NLS defines and routines
#ifndef Ava_WIN32_NLS
#	define NONLS
#endif

// OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#ifndef Ava_WIN32_OPENFILE
#	define NOOPENFILE
#endif

// Profiler interface.
#ifndef Ava_WIN32_PROFILER
#	define NOPROFILER
#endif

// Binary and Tertiary raster ops
#ifndef Ava_WIN32_RASTEROPS
#	define NORASTEROPS
#endif

// SB_* and scrolling routines
#ifndef Ava_WIN32_SCROLL
#	define NOSCROLL
#endif

// All Service Controller routines, SERVICE_ equates, etc.
#ifndef Ava_WIN32_SERVICE
#	define NOSERVICE
#endif

// SW_*
#ifndef Ava_WIN32_SHOWWINDOW
#	define NOSHOWWINDOW
#endif

// Sound driver routines
#ifndef Ava_WIN32_SOUND
#	define NOSOUND
#endif

// SC_*
#ifndef Ava_WIN32_SYSCOMMANDS
#	define NOSYSCOMMANDS
#endif

// SM_*
#ifndef Ava_WIN32_SYSMETRICS
#	define NOSYSMETRICS
#endif

// typedef TEXTMETRIC and associated routines
#ifndef Ava_WIN32_TEXTMETRIC
#	define NOTEXTMETRIC
#endif

// All USER defines and routines
#ifndef Ava_WIN32_USER
#	define NOUSER
#endif

// VK_*
#ifndef Ava_WIN32_VIRTUALKEYCODES
#	define NOVIRTUALKEYCODES
#endif

// SetWindowsHook and WH_*
#ifndef Ava_WIN32_WH
#	define NOWH
#endif

// WM_* , EM_*, LB_*, CB_*
#ifndef Ava_WIN32_WINMESSAGES
#	define NOWINMESSAGES
#endif

// GWL_* , GCL_*, associated routines
#ifndef Ava_WIN32_WINOFFSETS
#	define NOWINOFFSETS
#endif

// WS_* , CS_*, ES_*, LBS_*, SBS_*, CBS_*
#ifndef Ava_WIN32_WINSTYLES
#	define NOWINSTYLES
#endif

// OEM Resource values
#ifndef Ava_WIN32_OEMRESOURCE
#	define OEMRESOURCE
#endif

#define NOMINMAX

#include <Windows.h>

#undef NOMINMAX

#ifdef WIN32_LEAN_AND_MEAN
#	undef WIN32_LEAN_AND_MEAN
#endif

#ifdef NOATOM
#	undef NOATOM
#endif

#ifdef NOCLIPBOARD
#	undef NOCLIPBOARD
#endif

#ifdef NOCOLOR
#	undef NOCOLOR
#endif

#ifdef NOCOMM
#	undef NOCOMM
#endif

#ifdef NOCTLMGR
#	undef NOCTLMGR
#endif

#ifdef NODEFERWINDOWPOS
#	undef NODEFERWINDOWPOS
#endif

#ifdef NODRAWTEXT
#	undef NODRAWTEXT
#endif

#ifdef NOGDI
#	undef NOGDI
#endif

#ifdef NOGDICAPMASKS
#	undef NOGDICAPMASKS
#endif

#ifdef NOHELP
#	undef NOHELP
#endif

#ifdef NOICONS
#	undef NOICONS
#endif

#ifdef NOKANJI
#	undef NOKANJI
#endif

#ifdef NOKERNEL
#	undef NOKERNEL
#endif

#ifdef NOKEYSTATES
#	undef NOKEYSTATES
#endif

#ifdef NOMB
#	undef NOMB
#endif

#ifdef NOMCX
#	undef NOMCX
#endif

#ifdef NOMEMMGR
#	undef NOMEMMGR
#endif

#ifdef NOMENUS
#	undef NOMENUS
#endif

#ifdef NOMETAFILE
#	undef NOMETAFILE
#endif

#ifdef NOMSG
#	undef NOMSG
#endif

#ifdef NONLS
#	undef NONLS
#endif

#ifdef NOOPENFILE
#	undef NOOPENFILE
#endif

#ifdef NOPROFILER
#	undef NOPROFILER
#endif

#ifdef NORASTEROPS
#	undef NORASTEROPS
#endif

#ifdef NOSCROLL
#	undef NOSCROLL
#endif

#ifdef NOSERVICE
#	undef NOSERVICE
#endif

#ifdef NOSHOWWINDOW
#	undef NOSHOWWINDOW
#endif

#ifdef NOSOUND
#	undef NOSOUND
#endif

#ifdef NOSYSCOMMANDS
#	undef NOSYSCOMMANDS
#endif

#ifdef NOSYSMETRICS
#	undef NOSYSMETRICS
#endif

#ifdef NOTEXTMETRIC
#	undef NOTEXTMETRIC
#endif

#ifdef NOUSER
#	undef NOUSER
#endif

#ifdef NOVIRTUALKEYCODES
#	undef NOVIRTUALKEYCODES
#endif

#ifdef NOWH
#	undef NOWH
#endif

#ifdef NOWINMESSAGES
#	undef NOWINMESSAGES
#endif

#ifdef NOWINOFFSETS
#	undef NOWINOFFSETS
#endif

#ifdef NOWINSTYLES
#	undef NOWINSTYLES
#endif

#ifdef OEMRESOURCE
#	undef OEMRESOURCE
#endif
