################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/syscalls.c 

OBJS += \
./Inc/syscalls.o 

C_DEPS += \
./Inc/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/%.o: ../Inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L476xx -I"/home/giulio/INTECS/Workspace/JC3/Inc" -I"/home/giulio/INTECS/Workspace/JC3/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/home/giulio/INTECS/Workspace/JC3/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/home/giulio/INTECS/Workspace/JC3/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/home/giulio/INTECS/Workspace/JC3/Drivers/CMSIS/Include"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


