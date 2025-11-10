import serial
import time

# 让用户输入COM端口
com_port = input("请输入COM端口（例如：COM1）：")

try:
    # 初始化 UART 端口
    ser = serial.Serial(port=com_port, baudrate=115200, timeout=1)

    # 确保 UART 端口已打开
    if ser.isOpen():
        print("串口已打开")

    while True:
        # 发送数据到 MCU
        message = "你好，MCU！"
        ser.write(message.encode())
        print(f"已发送: {message}")

        # 接收来自 MCU 的数据
        received_data = ser.readline().decode().strip()
        if received_data:
            print(f"已接收: {received_data}")

        time.sleep(1)  # 等待1秒

except serial.SerialException as e:
    print(f"串口错误：{e}")
except KeyboardInterrupt:
    print("程序已停止")
finally:
    # 关闭 UART 端口
    if 'ser' in locals() and ser.isOpen():
        ser.close()
        print("串口已关闭")
