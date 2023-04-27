#include "knx_stack.h"

#include "layer1_physical.h"
#include "layer2_data_link.h"
#include "object_application_program.h"
#include "object_address_table.h"
#include "object_association_table.h"
#include "object_group_table.h"
#include "object_memory.h"

void Ph_Knx_Tim_Counter(int channel);
void Ph_Knx_Rx_Interrupt(void);

void knx_init(void)
{
    init_address_table_object();
    init_application_program();
    init_association_table();
    init_group_object_table();
    memory_read();
    Ph_Init();
}

void knx_loop(void)
{
    Ph_Loop();
    L_Loop();
}
