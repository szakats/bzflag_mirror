################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dialogs/Fl_Dialog.cpp \
../dialogs/Fl_Error.cpp \
../dialogs/MainMenu.cpp \
../dialogs/MasterConfigurationDialog.cpp \
../dialogs/WorldOptionsDialog.cpp 

OBJS += \
./dialogs/Fl_Dialog.o \
./dialogs/Fl_Error.o \
./dialogs/MainMenu.o \
./dialogs/MasterConfigurationDialog.o \
./dialogs/WorldOptionsDialog.o 

CPP_DEPS += \
./dialogs/Fl_Dialog.d \
./dialogs/Fl_Error.d \
./dialogs/MainMenu.d \
./dialogs/MasterConfigurationDialog.d \
./dialogs/WorldOptionsDialog.d 


# Each subdirectory must supply rules for building sources it contributes
dialogs/%.o: ../dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


