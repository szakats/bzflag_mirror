################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../widgets/QuickLabel.cpp \
../widgets/TransformWidget.cpp 

OBJS += \
./widgets/QuickLabel.o \
./widgets/TransformWidget.o 

CPP_DEPS += \
./widgets/QuickLabel.d \
./widgets/TransformWidget.d 


# Each subdirectory must supply rules for building sources it contributes
widgets/%.o: ../widgets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


