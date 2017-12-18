#include "msg_int.h"

msg_t* msg_init_int(void* content) {
    //viene creata una copia "privata" della stringa
    msg_t* new_msg = (msg_t*)malloc( sizeof(msg_t) );
    new_msg->content     = content;
    new_msg->msg_init    = msg_init_int;
    new_msg->msg_destroy = msg_destroy_int;
    new_msg->msg_copy    = msg_copy_int;
    return new_msg;
}
void msg_destroy_int(msg_t* msg) {
    free(msg);          // free struct
}
msg_t* msg_copy_int(msg_t* msg) {
    return msg->msg_init( msg->content );
}
