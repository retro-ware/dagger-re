/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008E6C5 */
#include "records.h"

extern struct record *found_object;
extern short object_find_group;
extern short object_find_index;
extern void object_foreach(struct record *, void (*)());
extern void object_find_item_cb(struct record *);

struct record *object_find_item(struct record *root, short group, short index)
{
    object_find_index = index;
    object_find_group = group;
    object_foreach(root, object_find_item_cb);
    if (object_find_index == -1)
        return found_object;
    return 0;
}
