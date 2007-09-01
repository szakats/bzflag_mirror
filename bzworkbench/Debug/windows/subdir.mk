################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../windows/ConsoleWindow.cpp \
../windows/EventHandlerCollection.cpp \
../windows/MainWindow.cpp \
../windows/RenderWindow.cpp \
../windows/View.cpp 

OBJS += \
./windows/ConsoleWindow.o \
./windows/EventHandlerCollection.o \
./windows/MainWindow.o \
./windows/RenderWindow.o \
./windows/View.o 

CPP_DEPS += \
./windows/ConsoleWindow.d \
./windows/EventHandlerCollection.d \
./windows/MainWindow.d \
./windows/RenderWindow.d \
./windows/View.d 


# Each subdirectory must supply rules for building sources it contributes
windows/%.o: ../windows/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


