import os

def create_folder(folder_name):
    """ 创建一个新的文件夹（如果它尚不存在）。"""
    try:
        # 检查文件夹是否已存在
        if not os.path.exists(folder_name):
            os.makedirs(folder_name)
            print(f"文件夹 '{folder_name}' 创建成功。")
        else:
            print(f"文件夹 '{folder_name}' 已经存在。")
    except Exception as e:
        print(f"创建文件夹时出错: {e}")

# 使用函数创建一个新的文件夹
folder_name = "C28"
create_folder(folder_name)