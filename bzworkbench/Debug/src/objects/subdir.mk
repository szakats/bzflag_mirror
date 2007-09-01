################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/objects/arc.cpp \
../src/objects/base.cpp \
../src/objects/box.cpp \
../src/objects/bz2object.cpp \
../src/objects/cone.cpp \
../src/objects/define.cpp \
../src/objects/dynamicColor.cpp \
../src/objects/group.cpp \
../src/objects/link.cpp \
../src/objects/material.cpp \
../src/objects/mesh.cpp \
../src/objects/meshbox.cpp \
../src/objects/meshpyr.cpp \
../src/objects/options.cpp \
../src/objects/physics.cpp \
../src/objects/pyramid.cpp \
../src/objects/sphere.cpp \
../src/objects/teleporter.cpp \
../src/objects/tetra.cpp \
../src/objects/texturematrix.cpp \
../src/objects/waterLevel.cpp \
../src/objects/weapon.cpp \
../src/objects/world.cpp \
../src/objects/zone.cpp 

OBJS += \
./src/objects/arc.o \
./src/objects/base.o \
./src/objects/box.o \
./src/objects/bz2object.o \
./src/objects/cone.o \
./src/objects/define.o \
./src/objects/dynamicColor.o \
./src/objects/group.o \
./src/objects/link.o \
./src/objects/material.o \
./src/objects/mesh.o \
./src/objects/meshbox.o \
./src/objects/meshpyr.o \
./src/objects/options.o \
./src/objects/physics.o \
./src/objects/pyramid.o \
./src/objects/sphere.o \
./src/objects/teleporter.o \
./src/objects/tetra.o \
./src/objects/texturematrix.o \
./src/objects/waterLevel.o \
./src/objects/weapon.o \
./src/objects/world.o \
./src/objects/zone.o 

CPP_DEPS += \
./src/objects/arc.d \
./src/objects/base.d \
./src/objects/box.d \
./src/objects/bz2object.d \
./src/objects/cone.d \
./src/objects/define.d \
./src/objects/dynamicColor.d \
./src/objects/group.d \
./src/objects/link.d \
./src/objects/material.d \
./src/objects/mesh.d \
./src/objects/meshbox.d \
./src/objects/meshpyr.d \
./src/objects/options.d \
./src/objects/physics.d \
./src/objects/pyramid.d \
./src/objects/sphere.d \
./src/objects/teleporter.d \
./src/objects/tetra.d \
./src/objects/texturematrix.d \
./src/objects/waterLevel.d \
./src/objects/weapon.d \
./src/objects/world.d \
./src/objects/zone.d 


# Each subdirectory must supply rules for building sources it contributes
src/objects/%.o: ../src/objects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


