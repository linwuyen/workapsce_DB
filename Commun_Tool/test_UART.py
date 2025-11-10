import serial
import time
import sys
sys.stdout.reconfigure(encoding='utf-8')

# 設置 UART 端口
SERIAL_PORT = "COM11"  # 修改成你的 UART 端口，例如 "/dev/ttyUSB0" (Linux)
BAUD_RATE = 115200

# 建立 serial 物件
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# 測試數據（Modbus RTU 格式：Slave ID = 1, Function Code = 3, Data = 4 bytes）
test_data = bytes([0x03, 0x03, 0x04, 0x12, 0x34, 0x56, 0x78])

# 送出數據
print(f"發送數據: {test_data.hex().upper()}")
ser.write(test_data)

# 給 MCU 一點時間處理數據
time.sleep(0.5)

# 嘗試接收數據
received_data = ser.read(16)  # 最多讀取 16 個 byte，可依實際情況調整

# 檢查接收的數據
if received_data:
    print(f"接收到數據: {received_data.hex().upper()}")
else:
    print("沒有收到數據，請檢查 UART 連線或 MCU 是否回應")

# 關閉 serial 端口
ser.close()
