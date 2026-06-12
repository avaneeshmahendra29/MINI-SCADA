import time
import math
import threading
from pymodbus.server import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext

# --- Datastore setup ---
coil_block = ModbusSequentialDataBlock(0, [0] * 10)
di_block   = ModbusSequentialDataBlock(0, [0] * 10)
ir_block   = ModbusSequentialDataBlock(0, [0] * 10)
hr_block   = ModbusSequentialDataBlock(0, [500] * 10)  # threshold default 500

store = ModbusSlaveContext(
    co=coil_block,
    di=di_block,
    ir=ir_block,
    hr=hr_block
)
context = ModbusServerContext(slaves=store, single=True)

# --- Simulation logic ---
def simulate(context):
    tick = 0
    while True:
        tick += 1
        slave = context[0x00]

        # Sine wave sensor value between 0 and 1000
        sensor_value = int(500 + 500 * math.sin(tick * 0.1))

        # Read threshold from holding register address 1
        threshold = slave.getValues(3, 1, count=1)[0]

        # Write sensor value to input register address 1
        slave.setValues(4, 1, [sensor_value])

        # Alarm logic
        alarm = 1 if sensor_value > threshold else 0

        # Write alarm to discrete input address 2
        slave.setValues(2, 2, [alarm])

        # Read running coil address 1
        running = slave.getValues(1, 1, count=1)[0]

        print(f"Sensor: {sensor_value:<6} | Threshold: {threshold:<6} | Alarm: {alarm} | Running: {running}")

        time.sleep(1)

# --- Start simulation thread ---
thread = threading.Thread(target=simulate, args=(context,), daemon=True)
thread.start()

# --- Start Modbus TCP server ---
print("Modbus TCP server started on 192.168.0.20:502")
StartTcpServer(context=context, address=("0.0.0.0", 502))