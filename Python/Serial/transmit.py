import serial
import time

# 配置串行端口參數
serial_port = 'COM7'  # Linux的端口名，Windows可能是'COM3'等
baud_rate = 9600  # 設定波特率，與微控制器設定相匹配
parity=serial.PARITY_NONE
stopbits=serial.STOPBITS_ONE
bytesize=serial.EIGHTBITS
timeout=1

# 創建串行端口連接
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# 發送指令到微控制器
command = "1111212124U!\n"
ser.write(command.encode())

# 給微控制器一點時間來響應
time.sleep(1)

# 讀取響應
response = ser.readline().decode().strip()
print("Response from MCU:", response)

# 關閉串行端口連接
ser.close()
