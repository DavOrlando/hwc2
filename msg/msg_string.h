#ifndef MSG_STRING_H
#define MSG_STRING_H

#include <string.h>
#include <stdlib.h>
#include "msg.h"
msg_t* msg_init_string(void*);
void msg_destroy_string(msg_t*);
msg_t* msg_copy_string(msg_t*);

#endif
