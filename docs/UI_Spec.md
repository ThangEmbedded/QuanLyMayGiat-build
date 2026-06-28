# Laundry Kiosk UI Specification

## 1. Project Overview

Embedded laundry kiosk application.

Target hardware:

- Raspberry Pi 4B
- 7 inch TFT Touch LCD
- Resolution: 800x480

Framework:

- Qt6 Widgets
- C++
- CMake

---

## 2. Main Features

### Customer Features

- View machine status
- Select washing machine
- Start washing program
- Stop washing program
- View remaining time
- Make payment
- View payment result

### Admin Features

- Monitor machine status
- View machine errors
- Enable/Disable machine
- View system logs

---

## 3. Screen List

### Screen 1 - Home Dashboard

Purpose:

Display all available washing machines.

Components:

- Header
- Current time
- Machine grid
- Machine status card

Machine States:

- Available
- Running
- Reserved
- Offline
- Error

Actions:

- Select machine

---

### Screen 2 - Machine Detail

Purpose:

Display detailed machine information.

Components:

- Machine ID
- Program Name
- Duration
- Price
- Status
- Start Button
- Stop Button
- Back Button

Actions:

- Start machine
- Stop machine
- Proceed to payment

---

### Screen 3 - Payment

Purpose:

Handle payment process.

Components:

- Machine ID
- Price
- QR Code
- Cancel Button
- Confirm Button

Actions:

- Confirm payment
- Cancel payment

---

### Screen 4 - Running Status

Purpose:

Display running machine information.

Components:

- Machine ID
- Program
- Remaining Time
- Progress Bar
- Status Indicator

Actions:

- Return Home

---

### Screen 5 - Admin Login

Purpose:

Access administrator functions.

Components:

- Username
- Password
- Login Button

---

### Screen 6 - Admin Dashboard

Purpose:

System management.

Components:

- Machine List
- Status Overview
- Error List
- Log Viewer

Actions:

- Enable machine
- Disable machine
- Reset machine

---

## 4. Navigation Flow

Home Dashboard
    ↓
Machine Detail
    ↓
Payment
    ↓
Running Status
    ↓
Home Dashboard

Admin Login
    ↓
Admin Dashboard

---

## 5. Reusable Widgets

### MachineCardWidget

Display:

- Machine ID
- Status
- Remaining Time

Used In:

- Home Dashboard
- Admin Dashboard

---

### StatusIndicatorWidget

States:

- Available
- Running
- Offline
- Error

---

### HeaderWidget

Display:

- Title
- Time
- Network Status

---

### ConfirmDialog

Display:

- Message
- Confirm
- Cancel

---

## 6. Qt Architecture

MainWindow
│
├── HomePage
├── MachineDetailPage
├── PaymentPage
├── RunningStatusPage
├── AdminLoginPage
└── AdminDashboardPage

Navigation:

QStackedWidget

---

## 7. Source Tree

src/
│
├── main.cpp
├── MainWindow.h
├── MainWindow.cpp
│
├── pages/
│   ├── HomePage.h
│   ├── MachineDetailPage.h
│   ├── PaymentPage.h
│   ├── RunningStatusPage.h
│   ├── AdminLoginPage.h
│   └── AdminDashboardPage.h
│
├── widgets/
│   ├── MachineCardWidget.h
│   ├── StatusIndicatorWidget.h
│   ├── HeaderWidget.h
│   └── ConfirmDialog.h
│
└── resources/

---

## 8. Design Rules

- Touch-friendly controls
- Large fonts
- Minimum button height: 48px
- Optimized for 800x480
- Single window application
- No overlapping dialogs where possible
- Fast screen switching

---

## 9. Build Requirements

Build System:

- CMake

Compiler:

- GCC

Qt Version:

- Qt 6.x

Target OS:

- Raspberry Pi OS
- Ubuntu 22.04
