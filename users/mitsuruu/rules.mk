SRC += mitsuruu.c

ifeq ($(strip $(MITSURUU_HHKB)), yes)
	SRC += hhkb/hhkb.c
	OPT_DEFS += -DHHKB_ENABLE
endif

ifeq ($(strip $(MITSURUU_RGBLIGHT_ENABLE)), yes)
	SRC += lighting/lighting.c
endif

ifeq ($(strip $(OS_DETECTION_ENABLE)), yes)
    SRC += os_detect/os_detect.c
endif

