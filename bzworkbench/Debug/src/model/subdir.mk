################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/BZWParser.cpp \
../src/model/Model.cpp \
../src/model/SceneBuilder.cpp 

OBJS += \
./src/model/BZWParser.o \
./src/model/Model.o \
./src/model/SceneBuilder.o 

CPP_DEPS += \
./src/model/BZWParser.d \
./src/model/Model.d \
./src/model/SceneBuilder.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/%.o: ../src/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


