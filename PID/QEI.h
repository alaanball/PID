void QEI_set_PPR(int ppr_val);
int QEI_get_RPM(void);
void QEI_Init(void (*CB_on_timer_ellapse) (void), void (*CB_on_dirn_change) (void));
int QEI_get_dirn(void);

