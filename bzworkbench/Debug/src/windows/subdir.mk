################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/windows/ConsoleWindow.cpp \
../src/windows/EventHandlerCollection.cpp \
../src/windows/MainWindow.cpp \
../src/windows/RenderWindow.cpp \
../src/windows/View.cpp 

CPP_DEPS += \
./src/windows/ConsoleWindow.d \
./src/windows/EventHandlerCollection.d \
./src/windows/MainWindow.d \
./src/windows/RenderWindow.d \
./src/windows/View.d 

OBJS += \
./src/windows/ConsoleWindow.o \
./src/windows/EventHandlerCollection.o \
./src/windows/MainWindow.o \
./src/windows/RenderWindow.o \
./src/windows/View.o 


# Each subdirectory must supply rules for building sources it contributes
src/windows/%.o: ../src/windows/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


