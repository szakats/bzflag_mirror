################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dialogs/Fl_Dialog.cpp \
../dialogs/QuickLabel.cpp 

OBJS += \
./dialogs/Fl_Dialog.o \
./dialogs/QuickLabel.o 

CPP_DEPS += \
./dialogs/Fl_Dialog.d \
./dialogs/QuickLabel.d 


# Each subdirectory must supply rules for building sources it contributes
dialogs/%.o: ../dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


