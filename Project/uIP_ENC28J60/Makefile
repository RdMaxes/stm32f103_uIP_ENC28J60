#====================================================================#
#Output files
EXECUTABLE=firmware.elf
BIN_IMAGE=firmware.bin

#======================================================================#
#Cross Compiler
HOST_CC=gcc
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
#============================================================================#
#Stm32F10x Standard Libraries
CMSIS=../../STM_Lib/CMSIS
ST=../../STM_Lib/STM32F10x_StdPeriph_Driver
STARTUP=../../STM_Lib/Startup/startup_stm32f10x_hd.s
#Source Files
SRC+=	./system_stm32f10x.c \
		$(ST)/src/misc.c \
		$(ST)/src/stm32f10x_rcc.c \
		$(ST)/src/stm32f10x_gpio.c \
		$(ST)/src/stm32f10x_usart.c \
		$(ST)/src/stm32f10x_spi.c \
		$(ST)/src/stm32f10x_tim.c

#Included Header Files
CFLAGS_INCLUDE=-I./ \
		-I$(CMSIS)/CM3/CoreSupport \
		-I$(CMSIS)/CM3/DeviceSupport/ST/STM32F10x \
		-I$(ST)/inc 
#============================================================================#
#User's Application
PERIPHERAL=../../Peripherals
HARDWARE=../../Hardware
USER_LIB=../../User_Lib

#User Source Files
SRC+=	./main.c \
		$(HARDWARE)/LED/led.c \
		$(HARDWARE)/ENC28J60/enc28j60.c \
		$(PERIPHERAL)/spi2/spi2.c \
		$(PERIPHERAL)/timer6/timer6.c \
		$(PERIPHERAL)/usart2/usart2.c \
		$(USER_LIB)/myprintf/myprintf.c

		#uIP-1.0 Lib
		SRC+=	$(USER_LIB)/uIP-1.0/uip/uip.c \
				$(USER_LIB)/uIP-1.0/uip/psock.c \
				$(USER_LIB)/uIP-1.0/uip/timer.c \
				$(USER_LIB)/uIP-1.0/uip/uip-fw.c \
				$(USER_LIB)/uIP-1.0/uip/uiplib.c \
				$(USER_LIB)/uIP-1.0/uip/uip_arp.c \
				$(USER_LIB)/uIP-1.0/uip/uip-split.c \
				$(USER_LIB)/uIP-1.0/uip/uip-neighbor.c 
		#uIP-UserApp
		SRC+=	$(USER_LIB)/uIP-UserApp/tapdev.c \
				$(USER_LIB)/uIP-UserApp/clock-arch.c \
				$(USER_LIB)/uIP-UserApp/tcp_demo.c \
				$(USER_LIB)/uIP-UserApp/tcp_client_demo.c \
				$(USER_LIB)/uIP-UserApp/tcp_server_demo.c 


#User Header Files
CFLAGS_INCLUDE+=-I./ \
		-I$(HARDWARE)/LED \
		-I$(HARDWARE)/ENC28J60 \
		-I$(PERIPHERAL)/spi2 \
		-I$(PERIPHERAL)/timer6 \
		-I$(PERIPHERAL)/usart2 \
		-I$(USER_LIB)/myprintf

		#uIP-1.0 Lib
		CFLAGS_INCLUDE+=	-I$(USER_LIB)/uIP-1.0/uip
		#uIP-UserApp
		CFLAGS_INCLUDE+=	-I$(USER_LIB)/uIP-UserApp
#======================================================================#
#Flags
CFLAGS_DEFINE= \
        -D USE_STDPERIPH_DRIVER \
		-D STM32F10X_HD \
#__CC_ARM
CFLAGS_OPTIMIZE= \
		-O2
CFLAGS_NEW_LIB_NANO= \
		--specs=nano.specs --specs=nosys.specs -u _printf_float
#warning
CFLAGS_WARNING= \
		-Wall \
		-Wno-unused-parameter \
		-Wextra \
		-Wdouble-promotion \
		-Werror=array-bounds \
		-Wfatal-errors \
		-Wmissing-prototypes \
		-Wbad-function-cast \
		-Wmissing-parameter-type
		
#Linker
CFLAGS = -Wl,-T,stm32_flash.ld \
		 -Wall -g -mlittle-endian -mthumb\
		 -mcpu=cortex-m3
CFLAGS+=${CFLAGS_INCLUDE} \
        ${CFLAGS_DEFINE} \
        ${CFLAGS_NEW_LIB_NANO} \
        ${CFLAGS_WARNING}

LDFLAGS=-lm
#============================================================================#
#Make rules

#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ = startup_stm32f10x_hd.o

$(STARTUP_OBJ): $(STARTUP)
	@$(CC) $(CFLAGS) $^ -c $(STARTUP)
	@echo '    CC $(STARTUP_OBJ)'

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo '    CC $(EXECUTABLE)'
PC_SIM:$(TEST_EXE)

$(TEST_EXE):$(HOST_SRC)
	$(HOST_CC) $(HOST_CFLAG) $^ -o $@
#Make clean
clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

#Make flash with Black Magic Probe(BMP)
flash:
	$(GDB) firmware.elf -x ./bmp_flash.gdb

#Start CGDB with Black Magic Probe(BMP)
cgdb:
	cgdb -d $(GDB) firmware.elf -x ./bmp_gdb.gdb

#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h
#============================================================================#

.PHONY:all clean flash cgdb astyle