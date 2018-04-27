#ifndef _COMMANDS_H
#define _COMMANDS_H

#define EP_OUT 0x00

#define PAGE_FWD 1
#define PAGE_BCK 2
#define CHAR_FWD 3
#define CHAR_BCK 4
#define PEN_DOWN 5
#define PEN_HOLD 6

#define MOVE_PAGE_FWD _IOW(0,0,unsigned long)
#define MOVE_PAGE_BCK _IOW(0,1,unsigned long)
#define MOVE_CHAR_FWD _IOW(0,2,unsigned long)
#define MOVE_CHAR_BCK _IOW(0,3,unsigned long)
#define MOVE_PEN_DOWN _IOW(0,4,unsigned long)
#define MOVE_PEN_HOLD _IOW(0,5,unsigned long)

#endif
