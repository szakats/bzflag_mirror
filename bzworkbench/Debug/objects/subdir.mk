################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../objects/arc.cpp \
../objects/base.cpp \
../objects/box.cpp \
../objects/bz2object.cpp \
../objects/cone.cpp \
../objects/define.cpp \
../objects/dynamicColor.cpp \
../objects/group.cpp \
../objects/link.cpp \
../objects/material.cpp \
../objects/mesh.cpp \
../objects/meshbox.cpp \
../objects/meshpyr.cpp \
../objects/options.cpp \
../objects/physics.cpp \
../objects/pyramid.cpp \
../objects/sphere.cpp \
../objects/teleporter.cpp \
../objects/tetra.cpp \
../objects/texturematrix.cpp \
../objects/waterLevel.cpp \
../objects/weapon.cpp \
../objects/world.cpp \
../objects/zone.cpp 

OBJS += \
./objects/arc.o \
./objects/base.o \
./objects/box.o \
./objects/bz2object.o \
./objects/cone.o \
./objects/define.o \
./objects/dynamicColor.o \
./objects/group.o \
./objects/link.o \
./objects/material.o \
./objects/mesh.o \
./objects/meshbox.o \
./objects/meshpyr.o \
./objects/options.o \
./objects/physics.o \
./objects/pyramid.o \
./objects/sphere.o \
./objects/teleporter.o \
./objects/tetra.o \
./objects/texturematrix.o \
./objects/waterLevel.o \
./objects/weapon.o \
./objects/world.o \
./objects/zone.o 

CPP_DEPS += \
./objects/arc.d \
./objects/base.d \
./objects/box.d \
./objects/bz2object.d \
./objects/cone.d \
./objects/define.d \
./objects/dynamicColor.d \
./objects/group.d \
./objects/link.d \
./objects/material.d \
./objects/mesh.d \
./objects/meshbox.d \
./objects/meshpyr.d \
./objects/options.d \
./objects/physics.d \
./objects/pyramid.d \
./objects/sphere.d \
./objects/teleporter.d \
./objects/tetra.d \
./objects/texturematrix.d \
./objects/waterLevel.d \
./objects/weapon.d \
./objects/world.d \
./objects/zone.d 


# Each subdirectory must supply rules for building sources it contributes
objects/%.o: ../objects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


