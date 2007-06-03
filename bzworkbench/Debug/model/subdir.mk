################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../model/Model.cpp \
../model/BZWParser.cpp

OBJS += \
./model/Model.o \
./model/BZWParser.o

CPP_DEPS += \
./model/Model.d \
./model/BZWParser.d


# Each subdirectory must supply rules for building sources it contributes
model/%.o: ../model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


