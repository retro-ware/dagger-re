/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0001B06F */
#include "records.h"

extern int shared_scratch;
extern struct faction *D_0019670C;
extern struct faction *D_0019671C;
extern int faction_subtree_search_r(struct faction *);

int faction_tree_relation(struct faction *faction, struct faction *other)
{
    struct faction *original;

    original = faction;
    D_0019671C = other;
    D_0019670C = faction->parent;
    shared_scratch = 0;
    if ((unsigned char)(faction_subtree_search_r(other->child) & 1))
        return 3;
    if (faction->parent != 0) {
        faction = faction->parent->child;
        while (faction != 0) {
            if (faction == other)
                return 2;
            faction = faction->next;
        }
    }
    faction = original;
    if (faction->parent != 0) {
        faction = faction->parent;
        while (faction != 0) {
            if (faction == other)
                return 1;
            faction = faction->parent;
        }
    }
    return 0;
}
