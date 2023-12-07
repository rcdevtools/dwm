/* See LICENSE file for copyright and license details. */

/* Multimedia keys... */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int extrabar           = 1;        /* 0 means no extra bar */
static const char statussep         = ';';      /* separator between statuses */
static const char *fonts[]          = {
	"Hurmit Nerd Font:style=Regular:pixelsize=16"
};

static const char fg_norm[]      = "#ded88d";
static const char fg_sel[]       = "#ded88d";
static const char bg_norm[]      = "#09141b";
static const char border_norm[]  = "#17384c";
static const char border_sel[]   = "#fca02f";
static const char *colors[][3]      = {
	/*               fg       bg          border   */
	[SchemeNorm] = { fg_norm, bg_norm,    border_norm },
	[SchemeSel]  = { fg_sel,  border_norm, border_sel },
};

/* tagging */
static const char *tags[] = { "", "󰖟", "", "", "", "󱎓", "󰫔", "", "󰺿" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 4;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor   ignoretransient */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,       0 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,       0 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "󰉧",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define CONTROL ControlMask /* Control key */
#define SHIFT ShiftMask     /* Shift key */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|SHIFT,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|SHIFT,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|CONTROL|SHIFT, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *passmenu[] =
	{ "passmenu2", "-m", dmenumon, "-g", "1", "-l", "5", NULL};
static const char *up_vol[]   = {"pamixer", "-i", "5", NULL};
static const char *down_vol[] = {"pamixer", "-d", "5", NULL};
static const char *mute_vol[] = {"pamixer", "--toggle-mute", NULL};
static const char *brighter[] = {"brightnessctl", "set", "5%+", NULL};
static const char *dimmer[]   = {"brightnessctl", "set", "5%-", NULL};
static const char *scrot[]    =
	{"scrot", "/home/awkless/pictures/screenshots/%Y-%m-%d-%T.jpg", NULL};
static const char *slock[]    = {"slock", NULL};
static const char *reboot[]   = {"reboot", NULL};
static const char *shutdown[] = {"shutdown", "-h", "now", NULL};

static const Key keys[] = {
	/* modifier       key        function        argument */
	{0, XF86XK_AudioMute,        spawn,          {.v = mute_vol}},
	{0, XF86XK_AudioLowerVolume, spawn,          {.v = down_vol}},
	{0, XF86XK_AudioRaiseVolume, spawn,          {.v = up_vol}},
	{ MODKEY,         XK_F2,     spawn,          {.v = down_vol}},
	{ MODKEY,         XK_F3,     spawn,          {.v = up_vol}},
	{ MODKEY,         XK_F1,     spawn,          {.v = mute_vol}},
	{ 0,              XK_Print,  spawn,          {.v = scrot}},
	{MODKEY,          XK_Escape, spawn,          {.v = slock}},
	{MODKEY| SHIFT,   XK_Escape, spawn,          {.v = reboot}},
	{MODKEY| CONTROL, XK_Escape, spawn,          {.v = shutdown}},
	{0, XF86XK_MonBrightnessDown,spawn,          {.v = dimmer}},
	{0, XF86XK_MonBrightnessUp,  spawn,          {.v = brighter}},
	{ MODKEY,         XK_F8,     spawn,          {.v = dimmer}},
	{ MODKEY,         XK_F9,     spawn,          {.v = brighter}},
	{ MODKEY,         XK_p,      spawn,          {.v = passmenu } },
	{ MODKEY,         XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY | SHIFT, XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,         XK_b,      togglebar,      {0} },
	{ MODKEY | SHIFT, XK_b,      toggleextrabar, {0} },
	{ MODKEY,         XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,         XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,         XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,         XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,         XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,         XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,         XK_Return, zoom,           {0} },
	{ MODKEY,         XK_Tab,    view,           {0} },
	{ MODKEY | SHIFT, XK_c,      killclient,     {0} },
	{ MODKEY,         XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,         XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,         XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,         XK_space,  setlayout,      {0} },
	{ MODKEY | SHIFT, XK_space,  togglefloating, {0} },
	{ MODKEY,         XK_0,      view,           {.ui = ~0 } },
	{ MODKEY | SHIFT, XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,         XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,         XK_period, focusmon,       {.i = +1 } },
	{ MODKEY | SHIFT, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY | SHIFT, XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
	{ MODKEY | SHIFT, XK_q,      quit, {0} },
	{ MODKEY,         XK_q,      quit, {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click               mask    button   function      argument */
	{ ClkLtSymbol,         0,      Button1, setlayout,      {0} },
	{ ClkLtSymbol,         0,      Button3, setlayout,      {.v=&layouts[2]} },
	{ ClkStatusText,       0,      Button2, spawn,          {.v = termcmd } },
	{ ClkExBarLeftStatus,  0,      Button2, spawn,          {.v = termcmd } },
	{ ClkExBarMiddle,      0,      Button2, spawn,          {.v = termcmd } },
	{ ClkExBarRightStatus, 0,      Button2, spawn,          {.v = termcmd } },
	{ ClkClientWin,        MODKEY, Button1, movemouse,      {0} },
	{ ClkClientWin,        MODKEY, Button2, togglefloating, {0} },
	{ ClkClientWin,        MODKEY, Button3, resizemouse,    {0} },
	{ ClkTagBar,           0,      Button1, view,           {0} },
	{ ClkTagBar,           0,      Button3, toggleview,     {0} },
	{ ClkTagBar,           MODKEY, Button1, tag,            {0} },
	{ ClkTagBar,           MODKEY, Button3, toggletag,      {0} },
};
