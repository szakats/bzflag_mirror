################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/windows/eventHandlers/subdir.mk
-include src/windows/subdir.mk
-include src/widgets/subdir.mk
-include src/render/subdir.mk
-include src/objects/subdir.mk
-include src/model/subdir.mk
-include src/subdir.mk
-include src/dialogs/subdir.mk
-include share/teleporter/subdir.mk
-include share/pyramid/subdir.mk
-include share/box/subdir.mk
-include share/base/subdir.mk
-include plugins/Win32CommonControls/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: bzworkbench

# Tool invocations
bzworkbench: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/lib -L/usr/local/lib/ -o"bzworkbench" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(OBJS)$(CC_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(C++_DEPS) bzworkbench
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
