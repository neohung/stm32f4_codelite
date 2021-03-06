# Project name

PROJ_NAME=neo_usb
OUTPATH=build

# Sources
# stm32f4xx_it.c system_stm32f4xx.c syscalls.c
SRCS = main.c 

# USB
#SRCS += usbd_usr.c usbd_cdc_vcp.c usbd_desc.c usb_bsp.c
SRCS += usb_bsp.c usbd_usr.c usbd_desc.c usbd_cdc_vcp.c
###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

BINPATH=
CC=$(BINPATH)arm-none-eabi-gcc
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
SIZE=$(BINPATH)arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O2 -Wall -T$(LINKFILE)
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

# Includes
CFLAGS += -Iinc -Istartup -Istartup/cmsis
#CFLAGS += -Ilib/Conf

# Library paths
LIBPATHS = -Llib/StdPeriph -Llib/USB_Device/Core
LIBPATHS += -Llib/USB_Device/Class/cdc -Llib/USB_OTG

# Libraries to link
#LIBS = -lm -lstdperiph -lusbdevcore -lusbdevcdc -lusbcore
#Add --specs=rdimon.specs for sprintf function
LIBS = -lstdperiph -lm -lusbdevcore -lusbcore -lusbdevcdc --specs=rdimon.specs

# Extra includes
CFLAGS += -Ilib/StdPeriph/inc
CFLAGS += -Ilib/USB_OTG/inc
CFLAGS += -Ilib/USB_Device/Core/inc
CFLAGS += -Ilib/USB_Device/Class/cdc/inc

# add startup file to build
SRCS += startup/system_stm32f4xx.c startup/startup_stm32f4xx.s

#Link Script File
LINKFILE=startup/stm32_flash.ld

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C lib 
# Remove this line if you don't want to clean the libs as well
	
