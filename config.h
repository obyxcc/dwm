/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 20;       /* gaps between windows */
static const unsigned int snap      = 5;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 4;       /* 2 is the default spacing around the bar's font */
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 12;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Bold:size=12" };
static const char fg[]              = "#CDD6F4";
static const char bg[]              = "#1E1E2E";
static const char fg_sel[]          = "#CDD6F4";
static const char bg_sel[]          = "#303446";
static const char border[]          = "#1E1E2E";
static const char border_sel[]      = "#CDD6F4";
static const char *colors[][3]      = {
	/*                   fg         bg         border   */
	[SchemeNorm]     = { fg,        bg,        border },
	[SchemeSel]      = { fg_sel, 	  bg_sel,    border_sel },
	[SchemeStatus]   = { fg,        bg,        border }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { fg_sel,    bg_sel,    border }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm] = { fg,        bg,        border }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { fg,        bg,        border }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm] = { fg,        bg,        border }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* autostart applications */
static const char *const autostart[] = {
	"autorandr", "-c", NULL,
	"picom", "-b", NULL,
	"dunst", NULL,
	"xwallpaper", "--daemon", "--zoom", "/home/cole/.config/wall", NULL,
	"dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "steam",    NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰕰",      tile },    /* first entry is default */
	{ "󰖲",      NULL },    /* no layout function means floating behavior */
	{ "󰝤",      monocle },
};

/* window following */
#define WFACTIVE '>'
#define WFINACTIVE 'v'
#define WFDEFAULT WFACTIVE

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "st", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-p", "run:", NULL };
static const char *dmenupcmd[] = { "dmenu_prun", NULL };
static const char *passmenucmd[]  = { "passmenu", NULL };
static const char *dmtunecmd[]  = { "dmtune", NULL };
static const char *brightnesscmd[2][4] = {
	{ "xbacklight", "-inc", "10%", NULL },
	{ "xbacklight", "-dec", "10%", NULL },
};
static const char *volumecmd[3][5] = {
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05+", NULL },
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05-", NULL },
	{ "wpctl", "set-mute", "@DEFAULT_SINK@", "toggle", NULL },
};
static const char *cmuscmd[3][3] = {
	{ "cmus-remote", "--pause", NULL },
	{ "cmus-remote", "--next", NULL },
	{ "cmus-remote", "--prev", NULL },
};
static const char *flameshotcmd[2][3] = {
	{ "flameshot", "gui", NULL },
	{ "flameshot", "full", NULL },
};

static const Key keys[] = {
	/* modifier                     key                       function     argument */
	{ MODKEY,                       XK_d,                     spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,                     spawn,          {.v = dmenupcmd } },
	{ MODKEY,                       XK_Return,                spawn,          {.v = termcmd } },
	{0,                             XF86XK_MonBrightnessUp,   spawn,          {.v = brightnesscmd[0]} },
  {0,                             XF86XK_MonBrightnessDown, spawn,          {.v = brightnesscmd[1]} },
	{0,                             XF86XK_AudioRaiseVolume,  spawn,          {.v = volumecmd[0]} },
  {0,                             XF86XK_AudioLowerVolume,  spawn,          {.v = volumecmd[1]} },
  {0,                             XF86XK_AudioMute,         spawn,          {.v = volumecmd[2]} },
	{0,                             XF86XK_AudioPlay,         spawn,          {.v = cmuscmd[0]} },
  {0,                             XF86XK_AudioNext,         spawn,          {.v = cmuscmd[1]} },
  {0,                             XF86XK_AudioPrev,         spawn,          {.v = cmuscmd[2]} },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_n,                     togglefollow,   {0} },
	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,                     movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                     movestack,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,                     rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,                     rotatestack,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_equal,                 incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_minus,                 incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,                 setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_period,                setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,											setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,											setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,                     togglefullscr,  {0} },
	{ MODKEY,                       XK_space,									setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,									togglefloating, {0} },
	{ MODKEY,                       XK_0,											view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,											tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_h,											focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,											focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,									tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,								tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,									setgaps,        {.i = -4 } },
	{ MODKEY,                       XK_equal,									setgaps,        {.i = +4 } },
	{ MODKEY|ShiftMask|ControlMask, XK_equal,									setgaps,        {.i = 0 } },
  { MODKEY|ShiftMask,             XK_equal,									setgaps,        {.i = gappx } },
	TAGKEYS(                        XK_1,											 							  0)
	TAGKEYS(                        XK_2,											 							  1)
	TAGKEYS(                        XK_3,											 							  2)
	TAGKEYS(                        XK_4,											 							  3)
	TAGKEYS(                        XK_5,											 							  4)
	TAGKEYS(                        XK_6,											 							  5)
	TAGKEYS(                        XK_7,											 							  6)
	TAGKEYS(                        XK_8,											 							  7)
	TAGKEYS(                        XK_9,											 							  8)
	{ MODKEY|ShiftMask,             XK_Escape,								quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_Escape,								quit,           {1} },

	/* program binds */
	{ MODKEY,                       XK_p,											spawn,          {.v = passmenucmd } },
	{ MODKEY|ShiftMask,             XK_t,											spawn,          {.v = dmtunecmd } },
	{ MODKEY,                       XK_Print,       					spawn,          {.v = flameshotcmd[0]} },
	{ MODKEY|ShiftMask,             XK_Print,       					spawn,          {.v = flameshotcmd[1]} },
	{ MODKEY|ShiftMask,             XK_n,											spawn,          SHCMD("$TERMINAL nnn")},
	{ MODKEY|ShiftMask,             XK_m,											spawn,          SHCMD("$TERMINAL cmus")},
	{ MODKEY|ShiftMask,             XK_c,											spawn,          SHCMD("$TERMINAL calcurse")},
	{ MODKEY|ShiftMask,             XK_a,											spawn,          SHCMD("$TERMINAL pulsemixer")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function           argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,         {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,         {.v = &layouts[2]} },
	{ ClkFollowSymbol,      0,              Button1,        togglefollow,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,              {0} },
	{ ClkStatusText,        0,              Button2,        spawn,             {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,         {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,       {0} },
	{ ClkTagBar,            0,              Button1,        view,              {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,        {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,               {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,         {0} },
};
