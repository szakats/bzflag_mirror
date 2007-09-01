################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dialogs/AdvancedOptionsDialog.cpp \
../dialogs/ConeConfigurationDialog.cpp \
../dialogs/ConfigurationMenu.cpp \
../dialogs/Fl_Dialog.cpp \
../dialogs/Fl_Error.cpp \
../dialogs/Fl_Tweak.cpp \
../dialogs/MainMenu.cpp \
../dialogs/MasterConfigurationDialog.cpp \
../dialogs/MenuBar.cpp \
../dialogs/WorldOptionsDialog.cpp 

OBJS += \
./dialogs/AdvancedOptionsDialog.o \
./dialogs/ConeConfigurationDialog.o \
./dialogs/ConfigurationMenu.o \
./dialogs/Fl_Dialog.o \
./dialogs/Fl_Error.o \
./dialogs/Fl_Tweak.o \
./dialogs/MainMenu.o \
./dialogs/MasterConfigurationDialog.o \
./dialogs/MenuBar.o \
./dialogs/WorldOptionsDialog.o 

CPP_DEPS += \
./dialogs/AdvancedOptionsDialog.d \
./dialogs/ConeConfigurationDialog.d \
./dialogs/ConfigurationMenu.d \
./dialogs/Fl_Dialog.d \
./dialogs/Fl_Error.d \
./dialogs/Fl_Tweak.d \
./dialogs/MainMenu.d \
./dialogs/MasterConfigurationDialog.d \
./dialogs/MenuBar.d \
./dialogs/WorldOptionsDialog.d 


# Each subdirectory must supply rules for building sources it contributes
dialogs/%.o: ../dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


