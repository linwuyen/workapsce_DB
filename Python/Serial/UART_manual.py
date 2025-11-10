import serial
import time

# 从用户输入获取串口配置
port = input("請輸入連接埠名稱（例如 COM3 或 /dev/ttyUSB0）: ")
baudrate = int(input("請輸入波特率（例如 9600）: "))
parity = input("請輸入奇偶校驗位元（N - 無，E - 偶校驗，O - 奇校驗）: ").upper()
stopbits = int(input("請輸入停止位元（1 或 2）: "))
bytesize = int(input("請輸入資料位元（5, 6, 7, 或 8）: "))
timeout = int(input("請輸入逾時時間（秒）: "))

# 将用户输入转换为 pySerial 需要的格式
if parity == 'N':
    parity = serial.PARITY_NONE
elif parity == 'E':
    parity = serial.PARITY_EVEN
elif parity == 'O':
    parity = serial.PARITY_ODD

if stopbits == 1:
    stopbits = serial.STOPBITS_ONE
elif stopbits == 2:
    stopbits = serial.STOPBITS_TWO

if bytesize == 5:
    bytesize = serial.FIVEBITS
elif bytesize == 6:
    bytesize = serial.SIXBITS
elif bytesize == 7:
    bytesize = serial.SEVENBITS
elif bytesize == 8:
    bytesize = serial.EIGHTBITS

# 配置串口
ser = serial.Serial(
    port=port,
    baudrate=baudrate,
    parity=parity,
    stopbits=stopbits,
    bytesize=bytesize,
    timeout=timeout
)

if ser.isOpen():
     print("\n\n")
     print("\n串列埠已開啟")
     print(f"串列埠配置：\n埠：{ser.port}\n波特率：{ser.baudrate}\n奇偶校驗：{ser.parity}\n停止位元：{ser.stopbits}\n資料位元：{ser.bytesize}\n逾時：{ser.timeout}")
try:
    # 向串口写入数据
    ser.write(b'Hello UART\n')
    time.sleep(1)

    # 循环读取串口数据
    while True:
        if ser.in_waiting > 0:
            receive_data = ser.readline()
            print("接收到的数据：", receive_data.decode('ascii'))
            break

except Exception as e:
    print("发生错误：", e)

finally:
    ser.close()  # 关闭串口
    print("串口已关闭")
