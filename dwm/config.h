/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;   /* border pixel of windows */
static const unsigned int snap      = 8;  /* snap pixel */
static const int showbar            = 1;   /* 0 means no bar */
static const int topbar             = 1;   /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

static const Rule rules[] = { NULL };

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size */
static const int nmaster     = 1;    /* number of clients in master area */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */
static const int resizehints = 1;    /* respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                        KEY,  view,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,            KEY,  toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,              KEY,  tag,        {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,  KEY,  toggletag,  {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]   = { "x-terminal-emulator", NULL };
static const char *chromecmd[] = { "chromium", NULL };
static const char *sshotcmd[]  = { "scrot", "%Y-%m-%d_$wx$h.png", "-f", "-s", NULL };

static const char * const vol_up     = "vol up\n";
static const char * const vol_down   = "vol down\n";
static const char * const vol_high   = "vol 0\n";
static const char * const vol_low    = "vol -1\n";
static const char * const foo_rand   = "rand\n";
static const char * const foo_pause  = "pause\n";
static const char * const foo_prev   = "prev\n";
static const char * const foo_next   = "next\n";

static const int FoobarMask = ControlMask|Mod1Mask;

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,            XK_c,      spawn,          {.v = chromecmd } },
	{ MODKEY,            XK_p,      spawn,          {.v = dmenucmd  } },
	{ MODKEY,            XK_v,      spawn,          {.v = dmenucmd  } },
	{ MODKEY,            XK_Return, spawn,          {.v = termcmd   } },
	{ MODKEY,            XK_s,      spawn,          {.v = sshotcmd  } },

	/* Foobar volume controls */
	{ MODKEY,            XK_F5,     spawn,          {.v = vol_up    } },
	{ MODKEY,            XK_F6,     spawn,          {.v = vol_down  } },
	{ MODKEY,            XK_F7,     spawn,          {.v = vol_high  } },
	{ MODKEY,            XK_F8,     spawn,          {.v = vol_low   } },

	/* Foobar playback controls */
	{ ControlMask,       XK_grave,  foobarctl,      {.v = foo_pause } },
	{ FoobarMask,        XK_Down,   foobarctl,      {.v = foo_rand  } },
	{ FoobarMask,        XK_k,      foobarctl,      {.v = foo_rand  } },
	{ FoobarMask,        XK_Left,   foobarctl,      {.v = foo_prev  } },
	{ FoobarMask,        XK_j,      foobarctl,      {.v = foo_prev  } },
	{ FoobarMask,        XK_Right,  foobarctl,      {.v = foo_next  } },
	{ FoobarMask,        XK_l,      foobarctl,      {.v = foo_next  } },

	{ MODKEY,            XK_q,      killclient,     {0} },
	{ MODKEY,            XK_b,      togglebar,      {0} },

	/* Change the currently focused window: z=previous, x=next */
	{ MODKEY,            XK_z,      focusstack,     {.i = -1 } },
	{ MODKEY,            XK_x,      focusstack,     {.i = +1 } },

	/* Change the partitioning of the master area: h=left, l=right */
	{ MODKEY,            XK_comma,  setmfact,       {.f = -0.05} },
	{ MODKEY,            XK_period, setmfact,       {.f = +0.05} },
	/* Change the number of items in the master area: j=more, k=less */
	{ MODKEY|ShiftMask,  XK_comma,  incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_period, incnmaster,     {.i = +1 } },

	/* Meta-tag operations -- tag=change to last, 0=select all */
	{ MODKEY,            XK_Tab,    view,           {0} },
	{ MODKEY,            XK_0,      view,           {.ui = ~0 } },

	// TODO: decide what kayboard shortcut should be "make window foating".
	//{ MODKEY|ShiftMask,  XK_space,  togglefloating, {0} },

	{ MODKEY,            XK_a,      decview,        {0} },
	{ MODKEY,            XK_d,      incview,        {0} },

	/* Moving things between monitors */
	{ MODKEY,            XK_Left,   focusmon,       {.i = -1 } },
	{ MODKEY,            XK_j,      focusmon,       {.i = -1 } },
	{ MODKEY,            XK_Right,  focusmon,       {.i = +1 } },
	{ MODKEY,            XK_l,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,  XK_Left,   tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,XK_j,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_Right,  tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,XK_l,      tagmon,         {.i = +1 } },

	/* Select tags and move windows between them */
	TAGKEYS(             XK_1,           0)
	TAGKEYS(             XK_2,           1)
	TAGKEYS(             XK_3,           2)
	TAGKEYS(             XK_4,           3)
	TAGKEYS(             XK_5,           4)
	TAGKEYS(             XK_6,           5)

	/* Quit dwm with Win+Shift+Q */
	{ MODKEY|ShiftMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = termcmd } },

	/* Clicking on the tag bar */
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },

	/* Clicking on a window with Win key held */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};
