import serial
import time

# 初始化 UART 端口，根據你的實際 COM 端口修改
ser = serial.Serial(port='COM12', baudrate=115200, timeout=1)

# 確保 UART 端口已打開
if ser.isOpen():
    print("Serial port is open")

# 發送 "HELLO" 字符串
message = "HELLO world"
ser.write(message.encode())  # 將字串轉換為字節發送
print(f"Sent: {message}")

# 關閉 UART 端口
ser.close()
