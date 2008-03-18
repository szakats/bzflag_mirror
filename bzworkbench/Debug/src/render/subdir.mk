################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/render/GeometryExtractorVisitor.cpp \
../src/render/Ground.cpp \
../src/render/Selection.cpp \
../src/render/TextureRepeaterVisitor.cpp 

CPP_DEPS += \
./src/render/GeometryExtractorVisitor.d \
./src/render/Ground.d \
./src/render/Selection.d \
./src/render/TextureRepeaterVisitor.d 

OBJS += \
./src/render/GeometryExtractorVisitor.o \
./src/render/Ground.o \
./src/render/Selection.o \
./src/render/TextureRepeaterVisitor.o 


# Each subdirectory must supply rules for building sources it contributes
src/render/%.o: ../src/render/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


