#ifndef __WashingMachine__
#define __WashingMachine__
extern char wm_mode(void);
extern char wm_water_size(void);
extern void wm_op(void);
extern void wm_water_in(int, int);
extern void wm_wash(int);
extern void wm_drain(int, int);
extern void wm_water_out(int);
extern void wm_done(void);
extern void wm_op_stard(void);
extern void wm_op_largeClothing(void);
extern void wm_op_sweater(void);
extern void wm_op_fastWash(void);
#endif

