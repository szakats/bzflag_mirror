################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../plugins/Win32CommonControls/Win32CommonControls.cpp 

CPP_DEPS += \
./plugins/Win32CommonControls/Win32CommonControls.d 

OBJS += \
./plugins/Win32CommonControls/Win32CommonControls.o 


# Each subdirectory must supply rules for building sources it contributes
plugins/Win32CommonControls/%.o: ../plugins/Win32CommonControls/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


