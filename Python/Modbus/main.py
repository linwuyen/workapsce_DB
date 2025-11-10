from pymodbus.client import ModbusSerialClient

# 創建一個 Modbus RTU 客戶端（透過 UART/串口）
client = ModbusSerialClient(
    port='COM12',      # 你的 UART 端口名稱，Windows 下為 COMx，Linux 下通常為 /dev/ttyUSBx
    baudrate=115200,    # 波特率，根據你的設備設定
    parity='N',       # 奇偶校驗: 'N' (無校驗), 'E' (偶校驗), 'O' (奇校驗)
    stopbits=1,
    bytesize=8,
    timeout=1         # 超時設置（秒）
)

# 連接到 Modbus Slave
connection = client.connect()
if connection:
    print("Connected to Modbus Slave")
    
    # 讀取保持寄存器（Function Code 0x03），從物理地址 0 開始，讀取 2 個寄存器
    result = client.read_holding_registers(0, 2)

    # 檢查是否有錯誤
    if result.isError():
        print(f"Error: {result}")
    else:
        print(f"Registers: {result.registers}")

    # 關閉 Modbus 連接
    client.close()
else:
    print("Failed to connect to Modbus Slave")
