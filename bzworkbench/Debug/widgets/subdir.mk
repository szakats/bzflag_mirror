################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/widgets/Console.cpp \
../src/widgets/Fl_ImageButton.cpp \
../src/widgets/MaterialWidget.cpp \
../src/widgets/QuickLabel.cpp \
../src/widgets/TransformWidget.cpp 

OBJS += \
./src/widgets/Console.o \
./src/widgets/Fl_ImageButton.o \
./src/widgets/MaterialWidget.o \
./src/widgets/QuickLabel.o \
./src/widgets/TransformWidget.o 

CPP_DEPS += \
./src/widgets/Console.d \
./src/widgets/Fl_ImageButton.d \
./src/widgets/MaterialWidget.d \
./src/widgets/QuickLabel.d \
./src/widgets/TransformWidget.d 


# Each subdirectory must supply rules for building sources it contributes
src/widgets/%.o: ../src/widgets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


