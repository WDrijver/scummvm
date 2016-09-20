MODULE := engines/chewy

MODULE_OBJS = \
	chewy.o \
	console.o \
	detection.o \
	graphics.o \
	resource.o \
	sound.o

# This module can be built as a plugin
ifeq ($(ENABLE_CHEWY), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
