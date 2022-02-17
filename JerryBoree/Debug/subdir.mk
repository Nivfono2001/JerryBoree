################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HashTable.c \
../Jerry.c \
../JerryBoreeMain.c \
../KeyValuePair.c \
../LinkedList.c \
../MultiValueHashTable.c 

OBJS += \
./HashTable.o \
./Jerry.o \
./JerryBoreeMain.o \
./KeyValuePair.o \
./LinkedList.o \
./MultiValueHashTable.o 

C_DEPS += \
./HashTable.d \
./Jerry.d \
./JerryBoreeMain.d \
./KeyValuePair.d \
./LinkedList.d \
./MultiValueHashTable.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


