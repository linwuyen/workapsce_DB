import serial
import time

# 配置串列埠
ser = serial.Serial(
    port='COM7',  # 串列埠名稱，根據你的系統進行修改（在 Linux 上可能是 '/dev/ttyUSB0'）
    baudrate=9600,  # 波特率，根據你的設備進行修改
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1  # 讀取超時時間
)

# 檢查串列埠是否已開啟
if ser.isOpen():
    print("串列埠已開啟")

try:
    # 向串列埠寫入數據
    ser.write(b'Hello UART\n')

    # 稍等一會兒，以便數據傳送完成
    time.sleep(1)

    # 初始化一個計數器
    count = 0
    max_count = 5  # 設定最大讀取次數

    # 讀取串列埠數據
    while count < max_count:
        if ser.in_waiting > 0:
            receive_data = ser.readline()
            print("接收到的數據：", receive_data.decode('ascii'))
            count += 1  # 更新計數器

except Exception as e:
    print("發生錯誤：", e)

finally:
    ser.close()  # 結束後關閉串列埠
    print("串列埠已關閉")
