/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 12;       /* gaps between windows */
static const unsigned int snap      = 5;        /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const unsigned int systrayiconsize = 32; /* systray icon size in px */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int showtitle          = 0;        /* 0 means no title */
static const int showtags           = 1;        /* 0 means no tags */
static const int showlayout         = 1;        /* 0 means no layout indicator */
static const int showwfsymbol       = 1;        /* 0 means no window follow symbol */
static const int showstatus         = 1;        /* 0 means no status bar */
static const int showfloating       = 1;        /* 0 means no floating indicator */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 4;        /* 2 is the default spacing around the bar's font */
static const int horizpadbar        = 12;        /* horizontal padding for statusbar */
static const int vertpadbar         = 12;       /* vertical padding for statusbar */
static const int vertpad            = 0;        /* vertical padding of bar */
static const int sidepad            = 0;        /* horizontal padding of bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Bold:size=12" };
static const char fg[]              = "#CDD6F4";
static const char bg[]              = "#181825";
static const char fg_sel[]          = "#CDD6F4";
static const char bg_sel[]          = "#313244";
static const char border[]          = "#1E1E2E";
static const char border_sel[]      = "#CDD6F4";
static const char *colors[][3]      = {
	/*                      fg         bg         border   */
	[SchemeNorm]        = { fg,        bg,        border },
	[SchemeSel]         = { fg_sel, 	 bg_sel,    border_sel },
	[SchemeStatus]      = { fg,        bg,        border }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsNorm]    = { fg,        bg,        border }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]     = { fg,        bg_sel,    border }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]    = { fg,        bg,        border }, // infobar middle  unselected {text,background,not used but cannot be empty}
	[SchemeScratchSel]  = { bg_sel,    fg,        border_sel  },
	[SchemeScratchNorm] = { bg_sel,    fg,        border },
};

/* autostart applications */
static const char *const autostart[] = {
	"autorandr", "-c", NULL,
	"dunst", NULL,
	"xwallpaper", "--daemon", "--zoom", "/home/cole/.config/wall", NULL,
	"dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "󰖟", "", "", "󰙯", "", "󰎈", "󰕝", "", "" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const char *tagsel[][2] = {
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
	{ "#CDD6F4", "#313244" },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title           tags mask     isfloating  isterminal  noswallow  monitor   scratch key */
	{ "steam",            NULL,       NULL,           0,            1,          0,          1,         -1,       0 },
	{ "flameshot",        NULL,       NULL,           0,            1,          0,          1,         -1,       0 },
	{ "solaar",           NULL,       NULL,           0,            1,          0,          1,         -1,       0 },
	{ "Msgcompose",       NULL,       NULL,           0,            1,          0,          1,         -1,       0 },
	{ "st-256color",      NULL,       NULL,           0,            0,          1,          0,         -1,       0 },
	{ NULL,               NULL,       "spterm",       0,            1,          0,          1,         -1,       's' },
	{ NULL,               NULL,       "spmix",        0,            1,          0,          1,         -1,       'a' },
	{ NULL,               NULL,       "spmus",        0,            1,          0,          1,         -1,       'm' },
	{ NULL,               NULL,       "spcldr",       0,            1,          0,          1,         -1,       'c' },
	{ NULL,               NULL,       "sprss",        0,            1,          0,          1,         -1,       'r' },
	{ NULL,               NULL,       "sptop",        0,            1,          0,          1,         -1,       'p' },
	{ NULL,               NULL,       "Event Tester", 0,            0,          0,          1,         -1,       0 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰕰",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
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

#define STATUSBAR "dwmblocks"

/* scratchpads */
/*First arg only serves to match against key in rules*/
static const char *sptermcmd[] = {"s", "st", "-t", "spterm", "-g", "144x41", NULL};
static const char *sptopcmd[] = {"p", "st", "-t", "sptop", "-g", "144x41", "-e", "htop", NULL};
static const char *spmixcmd[] = {"a", "st", "-t", "spmix", "-g", "144x41", "-e", "pulsemixer", NULL};
static const char *spmuscmd[] = {"m", "st", "-t", "spmus", "-g", "144x41", "-e", "cmus", NULL};
static const char *spcldrcmd[] = {"c", "st", "-t", "spcldr", "-g", "144x41", "-e", "calcurse", NULL};
static const char *sprsscmd[] = {"r", "st", "-t", "sprss", "-g", "144x41", "-e", "newsboat", NULL};

/* commands */
static const char *termcmd[]  = { "st", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-p", "run:", NULL };
static const char *dmenupcmd[] = { "dmenu_prun", NULL };
static const char *passmenucmd[]  = { "passmenu", NULL };
static const char *dmtunecmd[]  = { "dmtune", NULL };
static const char *fmcmd[]  = { "st", "-e", "nnn", NULL };
static const char *brightnesscmd[2][4] = {
	{ "brillo", "-A", "10", NULL },
	{ "brillo", "-U", "10", NULL },
};
static const char *volumecmd[3][5] = {
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05+", NULL },
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05-", NULL },
	{ "wpctl", "set-mute", "@DEFAULT_SINK@", "toggle", NULL },
};
static const char *playerctlcmd[3][3] = {
	{ "playerctl", "play-pause", NULL },
	{ "playerctl", "next", NULL },
	{ "playerctl", "previous", NULL },
};
static const char *flameshotcmd[] = { "flameshot", "gui", NULL };

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
	{0,                             XF86XK_AudioPlay,         spawn,          {.v = playerctlcmd[0]} },
  {0,                             XF86XK_AudioNext,         spawn,          {.v = playerctlcmd[1]} },
  {0,                             XF86XK_AudioPrev,         spawn,          {.v = playerctlcmd[2]} },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_n,                     togglefollow,   {0} },
	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,                     pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,                     pushup,         {0} },
	{ MODKEY,                       XK_Insert,                incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_Delete,                incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,                 setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_period,                setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,                     killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_x,                     killunsel,      {0} },
	{ MODKEY|ControlMask,           XK_x,                     killallunsel,   {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_x,                     killall,        {0} },
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
	{ MODKEY|ControlMask,           XK_b,                     togglealttag,   {0} },
	{ MODKEY,                       XK_minus,									setgaps,        {.i = -4 } },
	{ MODKEY,                       XK_equal,									setgaps,        {.i = +4 } },
	{ MODKEY|ShiftMask,             XK_minus,									setgaps,        {.i = 0 } },
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

  /* scratchpad binds */
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = sptermcmd } },
	{ MODKEY|ShiftMask,             XK_grave,  removescratch,  {.v = sptermcmd } },
	{ MODKEY|ControlMask,           XK_grave,  setscratch,     {.v = sptermcmd } },
	{ MODKEY|ShiftMask,             XK_a,      togglescratch,  {.v = spmixcmd } },
	{ MODKEY|ShiftMask,             XK_m,      togglescratch,  {.v = spmuscmd } },
	{ MODKEY|ShiftMask,             XK_c,      togglescratch,  {.v = spcldrcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglescratch,  {.v = sprsscmd } },
	{ MODKEY|ShiftMask,             XK_p,      togglescratch,  {.v = sptopcmd } },

	/* program binds */
	{ MODKEY|ShiftMask,             XK_n,											spawn,          {.v = fmcmd } },
	{ MODKEY,                       XK_p,											spawn,          {.v = passmenucmd } },
	{ MODKEY|ShiftMask,             XK_t,											spawn,          {.v = dmtunecmd } },
	{ MODKEY,                       XK_Print,       					spawn,          {.v = flameshotcmd } },
};

/* button definitions */
/* click can be ClkFollowSymbol, ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function           argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,         {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,         {.v = &layouts[2]} },
	{ ClkFollowSymbol,      0,              Button1,        togglefollow,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,              {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,      {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,      {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,      {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,         {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,       {0} },
	{ ClkTagBar,            0,              Button1,        view,              {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,        {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,               {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,         {0} },
};
