################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c \
../misc_functions.c \
../SPI_lib.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o \
misc_functions.o \
SPI_lib.o

OBJS_AS_ARGS +=  \
main.o \
misc_functions.o \
SPI_lib.o

C_DEPS +=  \
main.d \
misc_functions.d \
SPI_lib.d

C_DEPS_AS_ARGS +=  \
main.d \
misc_functions.d \
SPI_lib.d

OUTPUT_FILE_PATH +=AT90CAN_SHB.elf

OUTPUT_FILE_PATH_AS_ARGS +=AT90CAN_SHB.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=at90can128 -B "C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\at90can128" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./misc_functions.o: .././misc_functions.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=at90can128 -B "C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\at90can128" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./SPI_lib.o: .././SPI_lib.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=at90can128 -B "C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\at90can128" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="AT90CAN_SHB.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=at90can128 -B "C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\at90can128"  
	@echo Finished building target: $@
	"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "AT90CAN_SHB.elf" "AT90CAN_SHB.hex"
	"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "AT90CAN_SHB.elf" "AT90CAN_SHB.eep" || exit 0
	"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "AT90CAN_SHB.elf" > "AT90CAN_SHB.lss"
	"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "AT90CAN_SHB.elf" "AT90CAN_SHB.srec"
	"C:\Users\Marek\Documents\Stralsund\Studium - Bachelor Motorsport Engineering\Studien_Programme\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "AT90CAN_SHB.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "AT90CAN_SHB.elf" "AT90CAN_SHB.a" "AT90CAN_SHB.hex" "AT90CAN_SHB.lss" "AT90CAN_SHB.eep" "AT90CAN_SHB.map" "AT90CAN_SHB.srec" "AT90CAN_SHB.usersignatures"
	