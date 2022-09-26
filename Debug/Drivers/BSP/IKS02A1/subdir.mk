################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/IKS02A1/iks02a1_motion_sensors.c \
../Drivers/BSP/IKS02A1/iks02a1_motion_sensors_ex.c 

OBJS += \
./Drivers/BSP/IKS02A1/iks02a1_motion_sensors.o \
./Drivers/BSP/IKS02A1/iks02a1_motion_sensors_ex.o 

C_DEPS += \
./Drivers/BSP/IKS02A1/iks02a1_motion_sensors.d \
./Drivers/BSP/IKS02A1/iks02a1_motion_sensors_ex.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/IKS02A1/%.o: ../Drivers/BSP/IKS02A1/%.c Drivers/BSP/IKS02A1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/iis2dlpc -I../Drivers/BSP/Components/iis2mdc -I../Drivers/BSP/Components/ism330dhcx -I../Drivers/BSP/IKS02A1 -I../Drivers/BSP/Components/Common -I../Middlewares/ST/STM32_MotionAC_Library/Inc -I../X-CUBE-MEMS1/Target -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-IKS02A1

clean-Drivers-2f-BSP-2f-IKS02A1:
	-$(RM) ./Drivers/BSP/IKS02A1/iks02a1_motion_sensors.d ./Drivers/BSP/IKS02A1/iks02a1_motion_sensors.o ./Drivers/BSP/IKS02A1/iks02a1_motion_sensors_ex.d ./Drivers/BSP/IKS02A1/iks02a1_motion_sensors_ex.o

.PHONY: clean-Drivers-2f-BSP-2f-IKS02A1

