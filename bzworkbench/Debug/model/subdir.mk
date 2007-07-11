################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../model/BZWParser.cpp \
../model/Model.cpp \
../model/SceneBuilder.cpp 

OBJS += \
./model/BZWParser.o \
./model/Model.o \
./model/SceneBuilder.o 

CPP_DEPS += \
./model/BZWParser.d \
./model/Model.d \
./model/SceneBuilder.d 


# Each subdirectory must supply rules for building sources it contributes
model/%.o: ../model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


