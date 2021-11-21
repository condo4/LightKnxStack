KNXSTACK_MX = $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
KNXSTACK_DIR = $(dir $(KNXSTACK_MX) )
KNXSTACK_NAME = $(lastword $(subst /, ,$(KNXSTACK_DIR)) )

$(info LightKnxStack configured with $(HW_MAC) in $(KNXSTACK_NAME))

KNXSTACK_SRC := $(wildcard $(KNXSTACK_DIR)/*.c)
KNXHW_SRC    := $(wildcard $(KNXSTACK_DIR)/$(HW_MAC)/*.c)
KNX_INC      := $(wildcard $(KNXSTACK_DIR)/*.h)
KNXOUT       := $(OUTPUT)/$(KNXSTACK_NAME)
KNXSTACK_OBJ := $(KNXSTACK_SRC:$(KNXSTACK_DIR)/%.c=$(OUTPUT)/$(KNXSTACK_NAME)/%.o) $(KNXHW_SRC:$(KNXSTACK_DIR)/$(HW_MAC)/%.c=$(OUTPUT)/$(KNXSTACK_NAME)/$(HW_MAC)/%.o)

OBJS += $(KNXSTACK_OBJ)

$(KNXOUT)/$(HW_MAC)/%.o: $(KNXSTACK_DIR)/$(HW_MAC)/%.c ${KNXBOARD_INC} $(MCUDIR)/Core/Inc/main.h
	@mkdir -p $(KNXOUT)/$(HW_MAC)
	$(CC) -o $@ -c $(CFLAGS) $(MCUINCLUDE) $<

$(KNXOUT)/%.o: $(KNXSTACK_DIR)/%.c ${KNXBOARD_INC} $(MCUDIR)/Core/Inc/main.h
	@mkdir -p $(KNXOUT)
	$(CC) -o $@ -c $(CFLAGS) $(MCUINCLUDE) $<
