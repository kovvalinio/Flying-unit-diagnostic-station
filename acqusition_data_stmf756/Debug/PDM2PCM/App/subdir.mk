################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PDM2PCM/App/pdm2pcm.c 

OBJS += \
./PDM2PCM/App/pdm2pcm.o 

C_DEPS += \
./PDM2PCM/App/pdm2pcm.d 


# Each subdirectory must supply rules for building sources it contributes
PDM2PCM/App/%.o PDM2PCM/App/%.su: ../PDM2PCM/App/%.c PDM2PCM/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../PDM2PCM/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../MEMS/App -I../MEMS/Target -I../Drivers/BSP/custom -I../Drivers/BSP/Components/iis2dlpc -I../Drivers/BSP/Components/iis2mdc -I../Drivers/BSP/Components/ism330dhcx -I../Drivers/BSP/IKS02A1 -I../Drivers/BSP/Components/Common -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-PDM2PCM-2f-App

clean-PDM2PCM-2f-App:
	-$(RM) ./PDM2PCM/App/pdm2pcm.d ./PDM2PCM/App/pdm2pcm.o ./PDM2PCM/App/pdm2pcm.su

.PHONY: clean-PDM2PCM-2f-App

