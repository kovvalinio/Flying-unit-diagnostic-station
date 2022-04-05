################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/iis2mdc/iis2mdc.c \
../Drivers/BSP/Components/iis2mdc/iis2mdc_reg.c 

OBJS += \
./Drivers/BSP/Components/iis2mdc/iis2mdc.o \
./Drivers/BSP/Components/iis2mdc/iis2mdc_reg.o 

C_DEPS += \
./Drivers/BSP/Components/iis2mdc/iis2mdc.d \
./Drivers/BSP/Components/iis2mdc/iis2mdc_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/iis2mdc/%.o: ../Drivers/BSP/Components/iis2mdc/%.c Drivers/BSP/Components/iis2mdc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/iis2dlpc -I../Drivers/BSP/Components/iis2mdc -I../Drivers/BSP/Components/ism330dhcx -I../Drivers/BSP/IKS02A1 -I../Drivers/BSP/Components/Common -I../Middlewares/ST/STM32_MotionAC_Library/Inc -I../MEMS/Target -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-iis2mdc

clean-Drivers-2f-BSP-2f-Components-2f-iis2mdc:
	-$(RM) ./Drivers/BSP/Components/iis2mdc/iis2mdc.d ./Drivers/BSP/Components/iis2mdc/iis2mdc.o ./Drivers/BSP/Components/iis2mdc/iis2mdc_reg.d ./Drivers/BSP/Components/iis2mdc/iis2mdc_reg.o

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-iis2mdc

