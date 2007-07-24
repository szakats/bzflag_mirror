################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../widgets/Console.cpp \
../widgets/Fl_ImageButton.cpp \
../widgets/QuickLabel.cpp \
../widgets/TransformWidget.cpp 

OBJS += \
./widgets/Console.o \
./widgets/Fl_ImageButton.o \
./widgets/QuickLabel.o \
./widgets/TransformWidget.o 

CPP_DEPS += \
./widgets/Console.d \
./widgets/Fl_ImageButton.d \
./widgets/QuickLabel.d \
./widgets/TransformWidget.d 


# Each subdirectory must supply rules for building sources it contributes
widgets/%.o: ../widgets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


