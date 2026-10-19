/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0002BD1E */
#include "records.h"

extern char str_invalid_quest_id[];
extern char str_invalid_quest_id_2[];
extern char str_bad_things_have_happened_3[];
extern char str_bad_things_have_happened[];
extern char str_bad_things_have_happened_2[];
extern struct record *nonworld_root;
extern iptr qbn_opcode_arg_counts;
extern void fatal_error(char *);
extern struct record *object_find_by_id(struct record *, iptr);

/* a resource's object pointer back to the object's id, checking it belongs to this quest */
#define FIX(o, msg) \
    if (o) { \
        if ((short)o->quest_id != quest->id) fatal_error(msg); \
        o = (struct record *)(uptr)o->id; \
    }

void quest_unlink_for_save(struct quest *quest)
{
    struct qbn_op *op;
    struct qbn_arg *arg;
    int unused1;
    int unused2;
    struct qbn_place *qbn_place;
    struct qbn_person *qbn_person;
    struct qbn_item *qbn_item;
    struct qbn_foe *foe;
    int unused3;
    int unused4;
    int j;
    int i;
    struct qbn_timer *timer;

    op = (struct qbn_op *)((char *)quest + quest->section_offsets[8]);
    for (i = 0; i < quest->section_counts[8]; i++, op++) {
        arg = op->args;
        op->arg_count = (*(unsigned char **)&qbn_opcode_arg_counts)[op->opcode] - '0';
        for (j = 0; j < op->arg_count; j++, arg++) {
            if (arg->record) arg->record -= (iptr)quest;
            if (arg->object) arg->object = (struct record *)(uptr)arg->object->id;
        }
    }
    qbn_person = (struct qbn_person *)((char *)quest + quest->section_offsets[3]);
    for (i = 0; i < quest->section_counts[3]; i++, qbn_person++) {
        FIX(qbn_person->object, str_invalid_quest_id)
    }
    qbn_place = (struct qbn_place *)((char *)quest + quest->section_offsets[4]);
    for (i = 0; i < quest->section_counts[4]; i++, qbn_place++) {
        if (qbn_place->object) {
            if ((short)qbn_place->object->quest_id != quest->id) fatal_error(str_invalid_quest_id_2);
            qbn_place->object = (struct record *)(uptr)qbn_place->object->id;
            if (object_find_by_id(nonworld_root, (iptr)qbn_place->object) == 0) fatal_error(str_bad_things_have_happened_3);
            if (qbn_place->object == 0) fatal_error(str_bad_things_have_happened);
            /* +0x03 the place's type (10: a fixed object), +0x04/+0x06 the object id's halves */
            if (qbn_place->scope == 10) {
                if ((unsigned)(((unsigned short)qbn_place->p2 & 0xffff) | (qbn_place->p1 << 16)) != (uptr)qbn_place->object)
                    fatal_error(str_bad_things_have_happened_2);
            }
        }
    }
    qbn_item = (struct qbn_item *)((char *)quest + quest->section_offsets[0]);
    for (i = 0; i < quest->section_counts[0]; i++, qbn_item++) {
        FIX(qbn_item->object, str_invalid_quest_id)
    }
    foe = (struct qbn_foe *)((char *)quest + quest->section_offsets[7]);
    for (i = 0; i < quest->section_counts[7]; i++, foe++) {
        FIX(foe->object, str_invalid_quest_id)
    }
    timer = (struct qbn_timer *)((char *)quest + quest->section_offsets[6]);
    for (i = 0; i < quest->section_counts[6]; i++, timer++) {
        FIX(timer->link1, str_invalid_quest_id)
        FIX(timer->link2, str_invalid_quest_id)
    }
}
