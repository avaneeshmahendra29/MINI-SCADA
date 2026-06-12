import time
from pymodbus.client import ModbusTcpClient

PI_IP = "192.168.0.20"
PORT = 502

client = ModbusTcpClient(PI_IP, port=PORT)
client.connect()
print(f"Connected to Modbus server at {PI_IP}:{PORT}")
print("-" * 55)

def read_all():
    # Read coils (FC01) — START, STOP, E-STOP
    coils = client.read_coils(1, count=3, slave=1)
    start  = coils.bits[0] if not coils.isError() else "ERR"
    stop   = coils.bits[1] if not coils.isError() else "ERR"
    estop  = coils.bits[2] if not coils.isError() else "ERR"

    # Read discrete inputs (FC02) — running status, fault/alarm
    di = client.read_discrete_inputs(1, count=2, slave=1)
    running = di.bits[0] if not di.isError() else "ERR"
    alarm   = di.bits[1] if not di.isError() else "ERR"

    # Read input register (FC04) — sensor value
    ir = client.read_input_registers(1, count=1, slave=1)
    sensor = ir.registers[0] if not ir.isError() else "ERR"

    # Read holding register (FC03) — threshold setpoint
    hr = client.read_holding_registers(1, count=1, slave=1)
    threshold = hr.registers[0] if not hr.isError() else "ERR"

    print(f"Sensor: {sensor:<6} Threshold: {threshold:<6} Alarm: {alarm:<5} Running: {running:<5} START: {start} STOP: {stop} ESTOP: {estop}")

try:
    while True:
        read_all()
        time.sleep(0.5)

except KeyboardInterrupt:
    print("\nSCADA client stopped.")
    client.close()