import serial
import time
from pymodbus.client import ModbusSerialClient

# 設定 UART 參數
serial_port = "COM11"  # Windows: COM3, Linux/macOS: "/dev/ttyUSB0"
baudrate = 115200
timeout = 1

# 建立 Modbus RTU Master 連接
client = ModbusSerialClient(
    port=serial_port,
    baudrate=baudrate,
    timeout=timeout,
    parity='E',
    stopbits=1,
    bytesize=8
)

# 嘗試連接
if not client.connect():
    print("無法連接到 Modbus Slave，請檢查 UART 設置。")
    exit()

# 發送 Modbus 讀取命令
slave_id = 3
start_address = 0
num_registers = 5

try:
    response = client.read_holding_registers(start_address, num_registers, slave_id)

    # **新增這一段，詳細顯示錯誤原因**
    if response.isError():
        print(f"讀取失敗，錯誤類型: {response}")
        print(f"錯誤詳細資訊: {response.__dict__}")  # 顯示 response 內部細節
    else:
        print(f"收到資料: {response.registers}")
except Exception as e:
    print(f"發生錯誤: {e}")

client.close()
