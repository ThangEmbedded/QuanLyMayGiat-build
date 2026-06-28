# Laundry Machine Controller

## Project Specification (ProjectSpec.md)

**Version:** 1.0

**Author:** Development Team

---

# 1. Project Overview

## 1.1 Introduction

This project develops an embedded Linux controller for commercial/self-service washing machines.

The controller is responsible for:

* Machine control
* User interaction
* Peripheral management
* Communication with backend server
* Error detection
* System monitoring

The software runs on Raspberry Pi 4B and uses Qt/QML to provide a modern touch-screen user interface.

This document is intended for both software developers and AI coding agents to understand the complete project architecture before implementation.

---

# 2. Design Goals

The project is designed with the following goals:

* High reliability
* Easy maintenance
* Modular architecture
* Hardware abstraction
* Easy migration to new hardware
* Long-term scalability
* Clear separation between UI and hardware
* AI-friendly source code organization

---

# 3. Hardware Platform

## Main CPU

* Raspberry Pi 4B
* Embedded Linux
* ARM Cortex-A72

---

## Display

* TFT LCD
* 7 inch
* Resolution: 800x480
* Capacitive Touch

---

## Communication Interfaces

### UART

Used for:

* Debug console
* Peripheral controller

UART0 is reserved for Linux debug when required.  
Application should avoid depending on UART0.

---

### I2C

Used for:

* ADC
* Sensor expansion
* Future peripherals

---

### GPIO

Used for:

* Relay
* Door lock
* Water valve
* Pump
* LEDs
* Status input

---

### MQTT

Used for communication with backend server.

Responsibilities:

* Upload machine status
* Upload alarms
* Receive remote commands
* OTA preparation (future)

---

# 4. Functional Requirements

## Machine Control

Software shall support:

* Idle
* Ready
* Running
* Pause
* Completed
* Error
* Maintenance

---

## Washing Program

The controller shall support multiple washing programs.

Each program contains:

* Water filling
* Washing
* Draining
* Rinsing
* Spinning

Each stage has configurable parameters.

---

## User Interface

Touch screen interface shall provide:

* Home screen
* Program selection
* Running status
* Remaining time
* Error display
* Settings
* Maintenance page

---

## Monitoring

The controller continuously monitors:

* Door status
* Water level
* Machine status
* Sensors
* GPIO status
* Communication status

---

## Alarm Handling

The software shall detect and report:

* Door open
* Water timeout
* Sensor failure
* Communication timeout
* Emergency stop
* Internal software error

---

# 5. Non-functional Requirements

## Reliability

The software should run continuously without restart.

Target uptime:

> 30 days minimum

---

## Boot Time

Target boot time:

< 20 seconds

---

## UI Response

Touch response:

< 100 ms

---

## Logging

All important operations shall be logged.

Including:

* Error
* Warning
* Machine event
* User operation

---

# 6. Software Stack

Operating System

Embedded Linux

Language

C++17

GUI

Qt 6 + QML

Build System

CMake

Compiler

GCC

Version Control

Git

---

# 7. Software Architecture

The project follows Layered Architecture.

```
UI (QML)

↓

Presentation Layer

↓

Business Logic

↓

Services

↓

HAL

↓

Linux Drivers

↓

Hardware
```

Each layer only communicates with adjacent layers.
No layer skipping is allowed.

---

# 8. Core Modules

The software is divided into independent modules.

## UI

Responsibilities:

* Screen display
* User interaction

No hardware access.

---

## Controllers

Responsibilities:

* Connect UI and business logic

No GPIO access.

---

## Business Layer

Responsibilities:

* Washing workflow
* State machine
* Alarm logic
* Program execution

This is the brain of the system.

---

## Services

Responsibilities:

* Device management
* Peripheral management

Examples:

* MotorService
* PumpService
* DoorService
* SensorService

---

## HAL

Responsibilities:

Abstract hardware operations.

Examples:

* GPIO
* UART
* ADC
* I2C

No business logic.

---

# 9. Coding Principles

The project follows these principles.

## Single Responsibility

Each class has one responsibility.

---

## Hardware Isolation

UI must never access GPIO.
Business must never access Linux driver directly.
Only HAL accesses hardware.

---

## Dependency Direction

```
UI

↓

Controller

↓

Business

↓

Service

↓

HAL
```

Reverse dependency is forbidden.

---

## Global Variables

Avoid global variables whenever possible.
Use dependency injection.

---

# 10. Source Tree

Recommended structure

```
src/

    app/

    ui/

    controllers/

    core/

    services/

    hal/

    drivers/

    common/

    resources/

    tests/

    docs/

    scripts/
```

---

# 11. Communication

Current protocols

* MQTT
* UART
* I2C

Future

* Modbus
* BLE
* Ethernet

---

# 12. State Machine

Typical workflow

```
Idle

↓

User Select Program

↓

Ready

↓

Lock Door

↓

Fill Water

↓

Wash

↓

Drain

↓

Rinse

↓

Spin

↓

Completed

↓

Idle
```

Error can occur at any state.

---

# 13. Logging Strategy

Levels

* TRACE
* DEBUG
* INFO
* WARN
* ERROR
* FATAL

All modules should use a common logging framework.

---

# 14. Future Expansion

The architecture should support:

* Multiple machine models
* OTA update
* Cloud management
* Payment gateway
* Camera integration
* Remote diagnostics
* Mobile application

---

# 15. AI Development Guidelines

This repository is designed for AI-assisted development.

Before implementing any feature, AI agents should:

1. Read ProjectSpec.md completely.
2. Read SoftwareArchitecture.md.
3. Follow CodingRules.md.
4. Never bypass architecture layers.
5. Never access hardware directly from UI.
6. Keep modules independent.
7. Prefer composition over inheritance.
8. Keep classes small and focused.
9. Write maintainable C++17 code.
10. Preserve modular architecture.

The goal is long-term maintainability rather than quick implementation.

---

# End of Specification