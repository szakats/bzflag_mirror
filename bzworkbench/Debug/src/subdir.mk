################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../src/OSFile.cxx 

CPP_SRCS += \
../src/BZWBAPI.cpp \
../src/BZWBPlugins.cpp \
../src/TextUtils.cpp \
../src/commonControls.cpp \
../src/main.cpp 

CXX_DEPS += \
./src/OSFile.d 

CPP_DEPS += \
./src/BZWBAPI.d \
./src/BZWBPlugins.d \
./src/TextUtils.d \
./src/commonControls.d \
./src/main.d 

OBJS += \
./src/BZWBAPI.o \
./src/BZWBPlugins.o \
./src/OSFile.o \
./src/TextUtils.o \
./src/commonControls.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


