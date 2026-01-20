SRC += mitsuruu.c

ifeq ($(strip $(MITSURUU_HHKB)), yes)
	SRC += hhkb/hhkb.c
	OPT_DEFS += -DHHKB_ENABLE
endif

ifeq ($(strip $(MITSURUU_SECRETS)), yes)
	SRC += secrets/secrets.c
	OPT_DEFS += -DSECRETS_ENABLE
endif

ifeq ($(strip $(MITSURUU_RGBLIGHT_ENABLE)), yes)
	SRC += lighting/lighting.c
endif
