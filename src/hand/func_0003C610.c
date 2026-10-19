/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0003C610 */
#include "records.h"

extern char shared_scratch[];
extern char *scratch_buffer;
extern signed char health_status_changed;
extern char health_status_disease_shown;

void health_status_add(struct record *object)
{
    struct disease *disease;

    if (object->type != 11) return;
    disease = &object->data.disease;
    if (disease->id >= 128 && health_status_disease_shown == 0 && disease->stage != 0) {
        health_status_changed = 1;
        (scratch_buffer)[*(int *)shared_scratch + 60000] = 117;
        (*(int *)shared_scratch)++;
        health_status_disease_shown = 1;
    } else if (disease->id < 100) {
        health_status_changed = 1;
        if ((object->flags & 0x8000) && disease->stage != 0) {
            (scratch_buffer)[*(int *)shared_scratch + 60000] = disease->id + 100;
            (*(int *)shared_scratch)++;
        }
    }
}
