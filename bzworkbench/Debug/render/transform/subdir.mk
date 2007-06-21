################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../render/transform/Orientation.cpp \
../render/transform/UVN.cpp \
../render/transform/scale.cpp \
../render/transform/shear.cpp \
../render/transform/shift.cpp \
../render/transform/spin.cpp 

OBJS += \
./render/transform/Orientation.o \
./render/transform/UVN.o \
./render/transform/scale.o \
./render/transform/shear.o \
./render/transform/shift.o \
./render/transform/spin.o 

CPP_DEPS += \
./render/transform/Orientation.d \
./render/transform/UVN.d \
./render/transform/scale.d \
./render/transform/shear.d \
./render/transform/shift.d \
./render/transform/spin.d 


# Each subdirectory must supply rules for building sources it contributes
render/transform/%.o: ../render/transform/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


