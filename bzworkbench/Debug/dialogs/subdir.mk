################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dialogs/Fl_Dialog.cpp \
../dialogs/MainMenu.cpp \
../dialogs/QuickLabel.cpp \
../dialogs/WorldOptionsDialog.cpp \
../dialogs/MasterConfigurationDialog.cpp

OBJS += \
./dialogs/Fl_Dialog.o \
./dialogs/MainMenu.o \
./dialogs/QuickLabel.o \
./dialogs/WorldOptionsDialog.o \
./dialogs/MasterConfigurationDialog.o

CPP_DEPS += \
./dialogs/Fl_Dialog.d \
./dialogs/MainMenu.d \
./dialogs/QuickLabel.d \
./dialogs/WorldOptionsDialog.d \
./dialogs/MasterConfigurationDialog.d


# Each subdirectory must supply rules for building sources it contributes
dialogs/%.o: ../dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


