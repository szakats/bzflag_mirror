################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/windows/eventHandlers/selectHandler.cpp 

CPP_DEPS += \
./src/windows/eventHandlers/selectHandler.d 

OBJS += \
./src/windows/eventHandlers/selectHandler.o 


# Each subdirectory must supply rules for building sources it contributes
src/windows/eventHandlers/%.o: ../src/windows/eventHandlers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


