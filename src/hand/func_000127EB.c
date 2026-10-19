/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000127EB */
#include "structs.h"
#include "clib.h"
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) profile_delete_item;
extern char str_profile_c[];
struct profile;
extern int profile_find_item(struct profile *, char *);
#pragma aux (sosconv) profile_find_item;


int profile_delete_item(struct profile *profile, iptr item)
{
    char *line;
    int length;

    length = 0;
    if ((short)profile_find_item(profile, (char *)item) == 0) return 0;
    line = profile->item;
    while (line[length] != 10) length++;
    length++;
    mc_memmove(line, line + length, (int)(profile->buffer + profile->length - (line + length)), str_profile_c, 1119, 4);
    profile->length -= length;
    profile->flags |= 128;
    return 1;
}
