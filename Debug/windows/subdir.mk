################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../windows/ConsoleWindow.cpp \
../windows/MainWindow.cpp \
../windows/RenderWindow.cpp 

OBJS += \
./windows/ConsoleWindow.o \
./windows/MainWindow.o \
./windows/RenderWindow.o 

CPP_DEPS += \
./windows/ConsoleWindow.d \
./windows/MainWindow.d \
./windows/RenderWindow.d 


# Each subdirectory must supply rules for building sources it contributes
windows/%.o: ../windows/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


