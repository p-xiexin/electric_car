#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "main.h"

#define SBUS_START_BYTE 0xa5
#define SBUS_END_BYTE 0x5a
#define SBUS_RX_BUF_NUM 16u
#define RC_FRAME_LENGTH 8u

typedef __packed struct
{
    int16_t ch[2];  // -660 ---- +660
    uint8_t s;
} RC_ctrl_t;


extern void remote_control_init(void);
extern const RC_ctrl_t *get_remote_control_point(void);

#endif
