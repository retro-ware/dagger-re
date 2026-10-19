/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00031B8A */
#include "records.h"
#include "clib.h"

#pragma pack(1)
#pragma pack()
extern char str_qinit_c[];
extern unsigned char person_flat_images[];
extern short person_faction_ids[];
extern struct record *nonworld_root;
extern struct record *player_object;
extern struct record *location_object;
extern struct location *current_location;
extern char *scratch_buffer;
extern struct record *quest_person_source;
extern unsigned char current_region;
extern struct loaded_location loaded_location;
extern char quest_location[];
extern int quest_location_door_count;
extern struct location_door *quest_location_doors;
extern struct record *quest_location_object;
extern struct location *quest_location_location;
extern char quest_init_retry;
extern unsigned char quest_init_local;
extern struct quest *current_quest;
extern struct faction *faction_find_type_in_region(short, short);
extern struct faction *faction_find(short);
extern struct faction *faction_random_of_type(unsigned char);
extern struct qbn_person *quest_record(struct quest *, int, int);
extern int quest_person_fits(struct location_door *, struct qbn_person *, struct building *, int);
extern struct faction *pick_random_of_three(struct faction **);
extern int faction_random_hostile_id(void);
extern int quest_object_in_use(int);
extern struct flat_cfg *flats_cfg_find(int);
extern int rand_range(int, int);
extern struct building *object_building(struct record *);
extern void location_free(char *);
extern void location_pick_random_town(char *);
extern struct record *object_create_child(struct record *, struct record *, int);
extern struct record *object_reparent(struct record *, struct record *);
extern int object_new_id(int);

int quest_init_person(struct qbn_person *qbn_person)
{
    struct location_door *doors;
    struct location_door *door;
    int door_count;
    int candidate_count;
    int i;
    int any_building;
    int tries;
    int npc_faction_id;
    struct record *loc_object;
    struct record *object;
    struct record *place_object;
    struct location *location;
    int *candidates;
    struct building *building;
    struct flat_cfg *flat_cfg;
    struct faction *faction;
    struct qbn_person *linked_person;

    npc_faction_id = 0;
    qbn_person->flags &= 0x0fff;
    if (qbn_person->kind == 21) {
        if (quest_person_source == 0 || quest_person_source->twin != 0)
            return 0;
        flat_cfg = flats_cfg_find(quest_person_source->image);
        object = object_create_child(nonworld_root, 0, 58);
        building = object_building(player_object);
        if (building != 0) {
            if (building->faction_id == 65535)
                building->faction_id = 510;
            if (building->faction_id == 0)
                building->faction_id = faction_find_type_in_region(current_region, 15)->id;
            mc_memcpy(&object->data, building, 26, str_qinit_c, 64, 4);
        } else if (quest_person_source->data.person.faction_id != 0) {
            object->data.building.faction_id = quest_person_source->data.person.faction_id;
        }
        if (quest_person_source->data.person.faction_id != 0)
            object->faction_id = quest_person_source->data.person.faction_id;
        else
            object->faction_id = building->faction_id;
        mc_strncpy(object->data.quest_npc.location_name, current_location->name, 4, str_qinit_c, 76);
        mc_memcpy(&object->x, &quest_person_source->x, 12, str_qinit_c, 77, 4);
        object->type = 41;
        object->image = quest_person_source->image;
        qbn_person->object = object;
        object->flags = 514;
        object->flags |= (int)(unsigned char)(flat_cfg->flags & 1) != 0 ? 4 : 0;
        object->home_region = (unsigned short)current_region;
        object->quest_id = current_quest->id;
        object->id = quest_person_source->id;
        object->repair_due = object->id;
        if ((int)(unsigned short)(object->flags & 4) != 0)
            *(RECORD_DATA(object) + 2) |= 16;
        else
            *(RECORD_DATA(object) + 2) &= ~16;
        return 1;
    }
    if (qbn_person->kind == -8) {
        object = object_create_child(nonworld_root, 0, 0);
        object->type = 65;
        object->image = 0;
        qbn_person->object = object;
        object->repair_due = 0;
        object->id = object_new_id(800);
        object->flags = 514;
        object->faction_id = qbn_person->faction_id;
        object->flags |= (int)(unsigned char)(flats_cfg_find(object->image)->flags & 1) != 0 ? 4 : 0;
        object->home_region = (unsigned short)current_region;
        object->quest_id = current_quest->id;
        return 1;
    }
    if (!(qbn_person->kind != -1 || (int)(short)(qbn_person->flags & 0x100) != 0)) {
        faction = faction_find(qbn_person->faction_id);
        if (faction->type == 4) {
            flat_cfg = flats_cfg_find(faction->flats[0]);
            object = object_create_child(nonworld_root, 0, 58);
            object->faction_id = faction->id;
            object->type = 41;
            object->image = faction->flats[0];
            object->flags = 514;
            object->flags |= (int)(unsigned char)(flat_cfg->flags & 1) != 0 ? 4 : 0;
            object->home_region = (unsigned short)current_region;
            object->quest_id = current_quest->id;
            qbn_person->object = object;
            object->repair_due = object->id = object_new_id(800);
            object->data.building.faction_id = qbn_person->faction_id;
            return 1;
        }
    }
    if (qbn_person->kind == -2) {
        if (qbn_person->faction_id == 10)
            npc_faction_id = person_faction_ids[rand_range(0, 9)];
        else
            npc_faction_id = faction_random_of_type(qbn_person->faction_id)->id;
        qbn_person->kind = -1;
    }
    if (!(qbn_person->kind != -4 || qbn_person->faction_id != 10000)) {
        i = faction_random_hostile_id();
        if (i == 0) {
            qbn_person->kind = -6;
        } else {
            qbn_person->kind = -1;
            qbn_person->faction_id = i;
        }
    }
    if (qbn_person->kind >= -5 && qbn_person->kind <= -3 || qbn_person->kind == -7) {
        linked_person = quest_record(current_quest, 3, qbn_person->faction_id);
        if (linked_person == 0) {
            qbn_person->object = 0;
            quest_init_retry++;
            return 1;
        }
        object = linked_person->object;
        if (object == 0) {
            qbn_person->object = 0;
            quest_init_retry++;
            return 1;
        }
        switch ((unsigned short)(qbn_person->kind + 7)) {
        case 0:
            ((unsigned char *)&qbn_person->flags)[1] &= 0xf9;
            *(short *)&qbn_person->flags |= (int)(unsigned short)(object->flags & 4) != 0 ? 1024 : 512;
            qbn_person->kind = -6;
            break;
        case 2:
            qbn_person->faction_id = object->data.building.faction_id;
            qbn_person->kind = -1;
            break;
        case 3:
            faction = faction_find(object->data.building.faction_id);
            if (faction != 0) {
                faction = pick_random_of_three(faction->enemies);
            } else {
                qbn_person->object = 0;
                qbn_person->kind = -5;
                quest_init_retry++;
                return 1;
            }
            if (faction != 0)
                qbn_person->faction_id = faction->id;
            else
                qbn_person->faction_id = 510;
            qbn_person->kind = -1;
            break;
        case 4:
            faction = faction_find(object->data.building.faction_id);
            if (faction != 0) {
                faction = pick_random_of_three(faction->allies);
            } else {
                qbn_person->object = 0;
                qbn_person->kind = -5;
                quest_init_retry++;
                return 1;
            }
            if (faction != 0)
                qbn_person->faction_id = faction->id;
            else
                qbn_person->faction_id = 510;
            qbn_person->kind = -1;
            break;
        }
    }
    if ((int)(short)(qbn_person->flags & 0xff) == 255)
        qbn_person->flags = (qbn_person->flags & 0xff00) + (rand() & 1);
    tries = 0;
retry:
    if ((int)(short)(qbn_person->flags & 0xff) == 0) {
        doors = loaded_location.doors;
        door_count = loaded_location.door_count;
        loc_object = location_object;
        location = current_location;
    } else {
        location_free(quest_location);
        location_pick_random_town(quest_location);
        door_count = quest_location_door_count;
        doors = quest_location_doors;
        loc_object = quest_location_object;
        location = quest_location_location;
    }
    candidates = (int *)scratch_buffer;
    candidate_count = 0;
    any_building = 0;
    if ((int)(short)(qbn_person->flags & 0x600) != 0)
        quest_init_local = (int)(short)(qbn_person->flags & 0x200) != 0 ? 1 : 0;
    else
        quest_init_local = rand() & 1;
    if ((int)(short)(qbn_person->flags & 0x100) == 0) {
        for (candidate_count = i = 0, door = doors; i < door_count; i++, door++) {
            if (door->building_index != 65535 && quest_person_fits(door, qbn_person, &location->buildings[door->building_index], 0) != 0)
                candidates[candidate_count++] = (door->building_index << 16) + door->id;
        }
    }
    if (qbn_person->kind != -6 && candidate_count == 0 || (int)(short)(qbn_person->flags & 0x100) != 0) {
        any_building = 1;
        for (i = 0, door = doors; i < door_count; i++, door++) {
            if ((int)(short)(qbn_person->flags & 0x100) != 0 && quest_person_fits(door, qbn_person, &location->buildings[door->building_index], 1) != 0)
                candidates[candidate_count++] = (door->building_index << 16) + door->id;
            else if (door->building_index != 65535 && quest_person_fits(door, qbn_person, &location->buildings[door->building_index], 1) != 0)
                candidates[candidate_count++] = (door->building_index << 16) + door->id;
        }
    }
    if (candidate_count == 0 && (int)(short)(qbn_person->flags & 0xff) == 0) {
        if (qbn_person->kind == -1)
            npc_faction_id = qbn_person->faction_id;
        if (qbn_person->kind == -2)
            npc_faction_id = faction_random_of_type(qbn_person->faction_id)->id;
        qbn_person->kind = -6;
    }
    if (candidate_count == 0 && qbn_person->kind != -6) {
        if (qbn_person->kind == -1)
            npc_faction_id = qbn_person->faction_id;
        if (qbn_person->kind == -2)
            npc_faction_id = faction_random_of_type(qbn_person->faction_id)->id;
        if (tries < 50)
            goto retry;
        qbn_person->kind = -6;
        tries = 0;
        goto retry;
    }
    if (candidate_count == 0)
        return 0;
    i = rand() % candidate_count;
    if (any_building != 0 && quest_object_in_use((loc_object->id & 0xffff0000) + (candidates[i] & 0xffff)) != 0 && ++tries < 100)
        goto retry;
    object = object_create_child(nonworld_root, 0, 58);
    mc_memcpy(&object->x, &loc_object->x, 12, str_qinit_c, 307, 4);
    mc_memcpy(&object->data, &location->buildings[(unsigned)candidates[i] >> 16], 26, str_qinit_c, 308, 4);
    mc_strncpy(object->data.quest_npc.location_name, location->name, 4, str_qinit_c, 309);
    if (object->data.building.faction_id == 0)
        object->data.building.faction_id = faction_find_type_in_region(current_region, 15)->id;
    if (npc_faction_id != 0)
        object->data.building.faction_id = npc_faction_id;
    else if (qbn_person->kind == -1)
        object->data.building.faction_id = qbn_person->faction_id;
    object->type = 41;
    object->image = 0;
    qbn_person->object = object;
    if (any_building != 0 && (int)(short)(qbn_person->flags & 0x100) != 0) {
        object->id = object_new_id((unsigned)loc_object->id >> 16);
        object->repair_due = object->id;
    } else {
        object->id = (loc_object->id & 0xffff0000) + (candidates[i] & 0xffff);
        object->repair_due = object->id;
    }
    object->flags = 514;
    if (npc_faction_id != 0) {
        object->image = faction_find(npc_faction_id)->flats[quest_init_local];
        object->faction_id = npc_faction_id;
    } else if (qbn_person->kind == -1) {
        object->image = faction_find(qbn_person->faction_id)->flats[quest_init_local];
        object->faction_id = qbn_person->faction_id;
    } else if ((short)object->data.building.faction_id > 0) {
        object->image = faction_find(object->data.building.faction_id)->flats[quest_init_local];
        object->faction_id = object->data.building.faction_id;
    } else {
        object->image = person_flat_images[quest_init_local * 10 + rand_range(0, 9)] + 23296;
        object->faction_id = 510;
    }
    flat_cfg = flats_cfg_find(object->image);
    object->flags |= (int)(unsigned char)(flat_cfg->flags & 1) != 0 ? 4 : 0;
    object->home_region = (unsigned short)current_region;
    object->quest_id = current_quest->id;
    if (any_building != 0 && (int)(short)(qbn_person->flags & 0x100) == 0) {
        place_object = object_create_child(nonworld_root, 0, 26);
        place_object->type = 40;
        place_object->id = object->id;
        place_object->quest_id = current_quest->id;
        object_reparent(place_object, object);
        object->id = object_new_id((unsigned)object->id >> 16);
    }
    location_free(quest_location);
    return 1;
}
