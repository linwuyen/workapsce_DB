import serial
import time

# 配置串列埠
ser = serial.Serial(
    port='COM7',  
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

if ser.isOpen():
    print("\nSerialOpen")
    # 打印串口配置信息
    print(f"\nPort：{ser.port}\nBaudrate：{ser.baudrate}\nParity：{ser.parity}\nStopbit：{ser.stopbits}\nDatabit：{ser.bytesize}\nTimeout：{ser.timeout}")

try:
    # 向串列埠寫入數據
    ser.write(b'Hello UART\n')
    time.sleep(1)

    # 循環讀取串列埠數據
    while True:
        if ser.in_waiting > 0:
            receive_data = ser.readline()
            print("接收到的數據：", receive_data.decode('ascii'))

            # 检查是否接收到特定字符串 'XXX'
            if 'XXX' in receive_data.decode('ascii'):
                print("接收到結束指令，準備關閉串列埠")
                break  # 跳出循环

except Exception as e:
    print("發生錯誤：", e)

finally:
    ser.close()  # 關閉串列埠
    print("串列埠已關閉")
