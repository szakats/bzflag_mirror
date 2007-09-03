################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../windows/eventHandlers/selectHandler.cpp 

OBJS += \
./windows/eventHandlers/selectHandler.o 

CPP_DEPS += \
./windows/eventHandlers/selectHandler.d 


# Each subdirectory must supply rules for building sources it contributes
windows/eventHandlers/%.o: ../windows/eventHandlers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -Wshadow -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


