# SCADA Modbus TCP Simulation

A simulation of an industrial SCADA system using a Raspberry Pi 4 as a virtual PLC (Modbus TCP server) and a Windows laptop as the SCADA client. All sensor data is simulated in Python — no physical sensors required.

---

## Project Overview

This project replicates the communication architecture of a real industrial SCADA system:

- **Raspberry Pi 4** acts as the PLC / Modbus TCP server
- **Laptop** acts as the SCADA master / Modbus TCP client
- **Direct ethernet cable** connects the two devices (no router, no WiFi)
- **pymodbus** handles the Modbus TCP protocol on both sides
- **Sensor data** is simulated using a sine wave (0–1000) on the Pi

---

## Architecture

```
Laptop (SCADA Client)                  Raspberry Pi (Virtual PLC)
─────────────────────                  ──────────────────────────
IP: 192.168.0.10                       IP: 192.168.0.20
                                       Port: 502 (Modbus TCP)
scada_client.py          Ethernet      modbus_server.py
  - polls every 500ms  ←────────────→   - sine wave sensor simulation
  - reads all registers  Modbus TCP     - alarm logic
  - displays live data                  - register management
```

---

## Register Map

| Address    | Type             | Name                  | Direction   | FC   |
|------------|------------------|-----------------------|-------------|------|
| Coil 0x0001 | Coil (R/W)      | START command         | SCADA → Pi  | FC05 |
| Coil 0x0002 | Coil (R/W)      | STOP command          | SCADA → Pi  | FC05 |
| Coil 0x0003 | Coil (R/W)      | EMERGENCY STOP        | SCADA → Pi  | FC05 |
| DI 0x0001  | Discrete Input   | Process running status| Pi → SCADA  | FC02 |
| DI 0x0002  | Discrete Input   | Fault / Alarm         | Pi → SCADA  | FC02 |
| IR 0x0001  | Input Register   | Sensor value (0–1000) | Pi → SCADA  | FC04 |
| HR 0x0001  | Holding Register | Alarm threshold       | SCADA → Pi  | FC03 |

---

## Alarm Logic

```
Sensor value (IR 0x0001)
        ↓
Compare with threshold (HR 0x0001, default 500)
        ↓
If sensor > threshold → set DI 0x0002 = 1 (Alarm triggered)
If sensor ≤ threshold → set DI 0x0002 = 0 (Normal)
```

---

## Hardware Requirements

| Component | Specification |
|---|---|
| Raspberry Pi | Pi 4 Model B (any RAM) |
| Power Supply | USB-C 5V 3A |
| MicroSD Card | 16GB+ Class 10 / A1 |
| Ethernet Cable | Cat5e or Cat6, 1–2 meter |
| Laptop | Windows, any modern spec |

---

## Software Requirements

**On Raspberry Pi:**
- Raspberry Pi OS Lite (64-bit)
- Python 3
- pymodbus 3.6.4

**On Laptop:**
- Windows 10/11
- Python 3.14+
- pymodbus 3.6.4

---

## Setup

### 1. Flash the SD Card

Download and install [Raspberry Pi Imager](https://raspberrypi.com/software).

- Device: Raspberry Pi 4
- OS: Raspberry Pi OS Lite (64-bit)
- Enable SSH, set username `pi` and a password in settings

After flashing, open `bootfs/cmdline.txt` and append at the end of the single line:
```
ip=192.168.0.20
```

### 2. Set Laptop Static IP

**Settings → Network & Internet → Ethernet → Edit**

| Field | Value |
|---|---|
| IP Address | 192.168.0.10 |
| Subnet Mask | 255.255.255.0 |
| Gateway | (leave blank) |

### 3. Assemble and Boot the Pi

1. Insert SD card into Pi (underside slot)
2. Connect ethernet cable
3. Connect USB-C power — Pi boots immediately
4. Wait 90 seconds

### 4. SSH into the Pi

```bash
ssh pi@192.168.0.20
```

### 5. Install Dependencies on Pi

```bash
sudo apt update && sudo apt upgrade -y
sudo pip3 install pymodbus==3.6.4 --break-system-packages
```

### 6. Install Dependencies on Laptop

```bash
pip install pymodbus==3.6.4
```

---

## Running the Project

### Start the Modbus Server (on Pi)

```bash
sudo python3 modbus_server.py
```

Expected output:
```
Modbus TCP server started on 192.168.0.20:502
Sensor: 499    | Threshold: 500 | Alarm: 0 | Running: 0
Sensor: 549    | Threshold: 500 | Alarm: 1 | Running: 0
```

### Start the SCADA Client (on Laptop)

```bash
python scada_client.py
```

Expected output:
```
Connected to Modbus server at 192.168.0.20:502
-------------------------------------------------------
Sensor: 549    Threshold: 500   Alarm: 1   Running: 0   START: False STOP: False ESTOP: False
Sensor: 598    Threshold: 500   Alarm: 1   Running: 0   START: False STOP: False ESTOP: False
Sensor: 470    Threshold: 500   Alarm: 0   Running: 0   START: False STOP: False ESTOP: False
```

---

## Shutting Down Safely

**Stop the client** (laptop PowerShell):
```
Ctrl + C
```

**Stop the server** (SSH terminal):
```
Ctrl + C
```

**Shutdown the Pi safely** (never just unplug):
```bash
sudo shutdown now
```
Wait 10 seconds for the LED to stop blinking, then unplug power.

---

## Project Structure

```
SCADA/
├── modbus_server.py      # Runs on Raspberry Pi — Modbus TCP server + simulation
├── scada_client.py       # Runs on laptop — Modbus TCP client + display
└── README.md
```

---

## How It Works

### Modbus TCP Protocol Stack

```
Application (pymodbus)
      ↓
Modbus TCP (port 502)
      ↓
TCP/IP
      ↓
Ethernet (direct cable)
```

### Communication Flow (every 500ms)

```
Laptop                              Pi
──────                              ──
FC04 → read IR[1]         →         return sensor value
FC02 → read DI[1:2]       →         return running, alarm
FC01 → read CO[1:3]       →         return start, stop, estop
FC03 → read HR[1]         →         return threshold
```

### Simulation Loop (every 1 second on Pi)

```python
sensor_value = int(500 + 500 * math.sin(tick * 0.1))  # sine wave 0-1000
alarm = 1 if sensor_value > threshold else 0            # alarm logic
write sensor → IR[1]
write alarm  → DI[2]
```

---

## Why Modbus TCP

Modbus TCP is an internationally standardised industrial protocol used in factories, power plants, water treatment facilities, and oil refineries worldwide. By implementing actual Modbus TCP rather than plain TCP sockets:

- Any commercial SCADA software (Ignition, WinCC, Wonderware) can connect to the Pi without code changes
- Any hardware PLC can replace the Pi without changes to the laptop client
- The register map mirrors real industrial deployments exactly
- The simulation is directly portable to a real sensor setup — only the data source line changes

---



---

