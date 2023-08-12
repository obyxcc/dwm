/* Add to dwm.c */
static void distributetags(const Arg *arg);

void
distributetags(const Arg *arg)
{
        unsigned int ui = 1;
        int i = 0;
        for (Client *c = selmon->clients; c; c = c->next) {
                c->tags = (ui << i) & TAGMASK;
                i = (i + 1) % LENGTH(tags);
        }
        focus(NULL);
        arrange(selmon);
}

/* Add similar to config.h
 * { MODKEY|ShiftMask, XK_t, distributetags, {0} },
 */
