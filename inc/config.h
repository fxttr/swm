/* See LICENSE file for copyright and license details. */

/* appearance */
#include "../src/movestack.c"
#include "../src/fibonacci.c"
#include <X11/XF86keysym.h>

#define CONKY 0
#define NUMCOLORS 9

static const char colors[NUMCOLORS][MAXCOLORS][9] = {
	// border - foreground - background
	{ "#212121", "#9D9DC1", "#121212" }, /* [0]  01 - Client normal */
	{ "#696969", "#6999FF", "#121212" }, /* [1]  02 - Client selected */
	{ "#212121", "#CF4F88", "#121212" }, /* [2]  03 - Client urgent */
	{ "#212121", "#53A6A6", "#121212" }, /* [3]  04 - Client occupied */
	{ "#212121", "#914E89", "#121212" }, /* [4]  05 - Red */
	{ "#212121", "#4779B3", "#121212" }, /* [5]  06 - Yellow */
	{ "#212121", "#47959E", "#121212" }, /* [6]  07 - Green */
	{ "#212121", "#7E62B3", "#121212" }, /* [7]  08 - Dark grey */
	{ "#212121", "#899CA1", "#121212" }, /* [8]  09 - grey */
};
static const unsigned int gappx = 6; /* gap pixel between windows */
enum showtab_modes {
	showtab_never,
	showtab_auto,
	showtab_nmodes,
	showtab_always
};
static const int showtab = showtab_always; /* Default tab bar show mode */
static const Bool toptab = False; /* False means bottom tab bar */
static const char *fonts[] = { "Terminus Bold:size=12" };
static const char dmenufont[] = "Terminus Bold:size=10";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[] = "#000000";
static const char normfgcolor[] = "#bbbbbb";
static const char selbordercolor[] = "#af8700";
static const char selbgcolor[] = "#000000";
static const char selfgcolor[] = "#29A3FA";
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1; /* 0 means bottom bar */

static const unsigned int systrayspacing = 2; /* systray spacing */
static const Bool showsystray = True; /* False means no systray */
static const Bool systraypinning = True;
static const Bool systraypinningfailfirst = True;

/* tagging */
static const char *tags[] = { "", "", "", "", "" };

static int def_layouts[1 + LENGTH(tags)] = { 0, 2, 0, 0, 1, 0, 0, 0, 0, 0 };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints =
	0; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]", tile }, /* first entry is default */
	{ "<><", NULL }, /* no layout function means floating behavior */
	{ "\\", monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
	{ MODKEY, KEY, view, { .ui = 1 << TAG } },                             \
		{ MODKEY | ControlMask, KEY, toggleview, { .ui = 1 << TAG } }, \
		{ MODKEY | ShiftMask, KEY, tag, { .ui = 1 << TAG } },          \
		{ MODKEY | ControlMask | ShiftMask,                            \
		  KEY,                                                         \
		  toggletag,                                                   \
		  { .ui = 1 << TAG } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                 \
	{                                          \
		.v = (const char *[])              \
		{                                  \
			"/bin/sh", "-c", cmd, NULL \
		}                                  \
	}
#define TERM "stc"

/* commands */
static char dmenumon[2] =
	"0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m",  dmenumon,	   "-fn",
				  dmenufont,   "-nb", normbgcolor, "-sb",
				  selbgcolor,  "-sf", selfgcolor,  NULL };
static const char *termcmd[] = { TERM, NULL };
static const char *filecmd[] = { TERM, "-e", "ranger", NULL };
static const char *editorcmd[] = { "emacs", NULL };
static const char *volraisecmd[] = { "pactl", "set-sink-volume",
				     "@DEFAULT_SINK@", "+5%", NULL };
static const char *vollowercmd[] = { "pactl", "set-sink-volume",
				     "@DEFAULT_SINK@", "-5%", NULL };
static const char *volmutecmd[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@",
				    "toggle", NULL };
static const char *slockcmd[] = { "slock", NULL };

static Key keys[] = {
	/* modifier                     key        					  function        argument */
	{ MODKEY, XK_p, spawn, { .v = dmenucmd } },
	{ MODKEY | ShiftMask, XK_Return, spawn, { .v = termcmd } },
	{ MODKEY | ShiftMask, XK_d, spawn, { .v = filecmd } },
	{ MODKEY | ShiftMask, XK_e, spawn, { .v = editorcmd } },
	{ MODKEY, XF86XK_AudioRaiseVolume, spawn, { .v = volraisecmd } },
	{ MODKEY, XF86XK_AudioLowerVolume, spawn, { .v = vollowercmd } },
	{ MODKEY, XF86XK_AudioMute, spawn, { .v = volmutecmd } },
	{ MODKEY | ShiftMask, XK_b, spawn, { .v = slockcmd } },
	{ MODKEY, XK_b, togglebar, { 0 } },
	{ MODKEY, XK_w, tabmode, { -1 } },
	{ MODKEY, XK_j, focusstack, { .i = +1 } },
	{ MODKEY, XK_k, focusstack, { .i = -1 } },
	{ MODKEY | ShiftMask, XK_l, incnmaster, { .i = +1 } },
	{ MODKEY | ShiftMask, XK_h, incnmaster, { .i = -1 } },
	{ MODKEY, XK_h, setmfact, { .f = -0.05 } },
	{ MODKEY, XK_l, setmfact, { .f = +0.05 } },
	{ MODKEY, XK_Return, zoom, { 0 } },
	{ MODKEY, XK_Tab, view, { 0 } },
	{ MODKEY | ShiftMask, XK_c, killclient, { 0 } },
	{ MODKEY, XK_s, setlayout, { .v = &layouts[0] } },
	{ MODKEY, XK_f, setlayout, { .v = &layouts[1] } },
	{ MODKEY, XK_t, setlayout, { .v = &layouts[2] } },
	{ MODKEY, XK_d, setlayout, { .v = &layouts[3] } },
	{ MODKEY, XK_space, setlayout, { 0 } },
	{ MODKEY | ShiftMask, XK_space, togglefloating, { 0 } },
	{ MODKEY, XK_0, view, { .ui = ~0 } },
	{ MODKEY | ShiftMask, XK_0, tag, { .ui = ~0 } },
	{ MODKEY, XK_comma, focusmon, { .i = -1 } },
	{ MODKEY, XK_period, focusmon, { .i = +1 } },
	{ MODKEY | ShiftMask, XK_comma, tagmon, { .i = -1 } },
	{ MODKEY | ShiftMask, XK_period, tagmon, { .i = +1 } },
	{ MODKEY | ShiftMask, XK_j, movestack, { .i = +1 } },
	{ MODKEY | ShiftMask, XK_k, movestack, { .i = -1 } },
	TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
		TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8,
									   7)
			TAGKEYS(XK_9,
				8){ MODKEY | ShiftMask, XK_q, quit, { 0 } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol, 0, Button1, setlayout, { 0 } },
	{ ClkLtSymbol, 0, Button3, setlayout, { .v = &layouts[2] } },
	{ ClkWinTitle, 0, Button2, zoom, { 0 } },
	{ ClkStatusText, 0, Button2, spawn, { .v = termcmd } },
	{ ClkClientWin, MODKEY, Button1, movemouse, { 0 } },
	{ ClkClientWin, MODKEY, Button2, togglefloating, { 0 } },
	{ ClkClientWin, MODKEY, Button3, resizemouse, { 0 } },
	{ ClkTagBar, 0, Button1, view, { 0 } },
	{ ClkTagBar, 0, Button3, toggleview, { 0 } },
	{ ClkTagBar, MODKEY, Button1, tag, { 0 } },
	{ ClkTagBar, MODKEY, Button3, toggletag, { 0 } },
	{ ClkTabBar, 0, Button1, focuswin, { 0 } },
};
