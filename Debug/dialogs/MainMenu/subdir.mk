################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dialogs/MainMenu/MainMenu.cpp 

OBJS += \
./dialogs/MainMenu/MainMenu.o 

CPP_DEPS += \
./dialogs/MainMenu/MainMenu.d 


# Each subdirectory must supply rules for building sources it contributes
dialogs/MainMenu/%.o: ../dialogs/MainMenu/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


