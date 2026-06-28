# Development Standard

**Project:** QuanLyMayGiat
**Version:** 1.0
**Language:** C++17
**Platform:** Raspberry Pi 4B (Linux)

---

# 1. Purpose

This document defines the software development standards for the QuanLyMayGiat project.

Every contributor (human or AI Agent) **MUST** follow these rules when creating, modifying or refactoring source code.

The primary objectives are:

* Maintainable source code
* Modular architecture
* Easy testing
* Easy hardware replacement
* Easy AI-assisted development

---

# 2. Software Architecture

The software shall follow a layered architecture.

```
Application
        │
        ▼
Service Layer
        │
        ▼
Driver Layer
        │
        ▼
HAL (Hardware Abstraction Layer)
        │
        ▼
Linux Hardware Interface
```

Layer dependency is only allowed from top to bottom.

Forbidden:

```
UI
    ↓
GPIO
```

Allowed:

```
UI
    ↓
LaundryService
    ↓
RelayDriver
    ↓
GPIO HAL
```

---

# 3. Folder Structure

```
src/

    app/

    services/

    drivers/

    common/

    ui/

tests/

docs/
```

Each driver shall have its own directory.

Example:

```
drivers/

    relay/

        relay.hpp

        relay.cpp

        relay_config.hpp

        README.md
```

---

# 4. Driver Design Rules

Each driver is responsible for ONE hardware only.

Example:

RelayDriver

Responsibilities

* initialize relay
* turn relay on
* turn relay off
* query relay state

Forbidden responsibilities

* MQTT
* Database
* GUI
* Business Logic

---

# 5. Hardware Abstraction Layer

Direct GPIO access is forbidden outside HAL.

Allowed

```
RelayDriver

↓

IGpio

↓

Linux GPIO
```

Forbidden

```
RelayDriver

↓

libgpiod
```

Every hardware interface shall have an abstract interface.

Example

```
IGpio

LinuxGpio

MockGpio
```

---

# 6. Dependency Rule

High-level modules shall not depend on low-level implementations.

Depend on interfaces only.

Use Dependency Injection whenever possible.

---

# 7. Naming Convention

Classes

```
RelayDriver
DoorLockDriver
```

Interfaces

```
IGpio
IAdc
IUart
```

Struct

```
RelayState
SensorData
```

Enum

```
enum class RelayStatus
```

Function

```
init()

read()

write()

enable()

disable()
```

Private member

```
gpio_

relay_state_

initialized_
```

Constant

```
constexpr uint32_t MAX_RETRY
```

---

# 8. Coding Rules

Use C++17.

Avoid raw pointers.

Prefer RAII.

No global variables.

No singleton unless justified.

No magic numbers.

Always use constexpr.

Use enum class instead of enum.

Avoid macros whenever possible.

Prefer composition over inheritance.

Maximum class responsibility: one module.

---

# 9. Error Handling

Functions interacting with hardware shall return status.

Example

```
bool init();

bool write();

bool open();
```

Do not terminate the application from inside drivers.

Drivers never call exit().

---

# 10. Logging

Drivers shall not print directly using printf().

Use project logging interface.

Example

```
LOG_INFO()

LOG_WARN()

LOG_ERROR()
```

---

# 11. Testing

Every module shall have

```
Unit Test

Hardware Test
```

Example

```
tests/

    unit/

    hardware/
```

Every driver must provide a hardware validation program.

---

# 12. Documentation

Every module shall contain

```
README.md
```

README shall include

* Purpose
* Public API
* Hardware dependency
* Example
* Test procedure
* Limitations

---

# 13. Public API

Public APIs shall remain stable.

Breaking API changes require updating:

* Documentation
* Tests
* Example code

---

# 14. Build

Host PC

Purpose

* Compile
* Static analysis
* Unit Test

No physical hardware required.

Raspberry Pi

Purpose

* Hardware Test
* Integration Test

---

# 15. AI Development Rules

AI Agent shall NOT

* change public APIs without request
* modify unrelated modules
* introduce new dependencies without approval
* bypass HAL
* access hardware directly from Service or UI

AI Agent SHALL

* preserve architecture
* preserve folder structure
* preserve naming convention
* write readable code
* add comments only when necessary
* update documentation if API changes
* create tests for new modules

---

# 16. Refactoring Rules

Refactoring shall NOT change external behaviour.

Allowed

* rename variables
* split classes
* improve readability
* reduce coupling

Forbidden

* modify hardware mapping
* modify communication protocol
* remove existing APIs without approval

---

# 17. Code Review Checklist

Before submitting code verify

[ ] Build successfully

[ ] No compiler warnings

[ ] Documentation updated

[ ] Test added

[ ] Naming follows standard

[ ] No direct hardware access outside HAL

[ ] No duplicated code

[ ] Single Responsibility Principle respected

[ ] Public API unchanged

[ ] Folder structure respected

---

# 18. Future Expansion

Future modules shall follow the same architecture.

Examples

* ADC Driver

* UART Driver

* I2C Driver

* EEPROM Driver

* MQTT Service

* Camera Service

No module is allowed to violate the architecture defined in this document.
