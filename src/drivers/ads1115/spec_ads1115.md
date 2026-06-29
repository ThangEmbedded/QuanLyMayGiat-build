# ADS1115 Driver Specification (Embedded C++)

## 1. Overview

ADS1115 is a 16-bit delta-sigma ADC with I2C interface.
It provides high precision analog-to-digital conversion with programmable gain amplifier (PGA) and internal reference.

This driver is designed for embedded Linux (Raspberry Pi) using an existing I2C abstraction layer.

---

## 2. Device Reference

### 2.1 Datasheet
- Texas Instruments ADS1113 / ADS1114 / ADS1115 Family
- Official documentation:
  https://www.ti.com/lit/ds/symlink/ads1115.pdf :contentReference[oaicite:0]{index=0}

---

### 2.2 Key Features

| Feature | Value |
|--------|------|
| Resolution | 16-bit |
| Interface | I2C |
| Channels | 4 single-ended / 2 differential |
| Sample Rate | 8 SPS to 860 SPS |
| PGA | ±256mV to ±6.144V |
| Mode | Single-shot / Continuous |
| Internal reference | Yes |
| Comparator | Yes |

---

## 3. Functional Description

ADS1115 performs analog-to-digital conversion using:
