import os

def delete_folder(folder_name):
    """如果存在则删除文件夹。"""
    if os.path.exists(folder_name):
        os.rmdir(folder_name)
        print(f"文件夹 '{folder_name}' 已被删除。")
    else:
        print(f"文件夹 '{folder_name}' 不存在。")

def create_folder_with_serial(base_folder_name):
    """创建一个带有流水号的新文件夹。"""
    serial_number = 1
    new_folder_name = f"{base_folder_name}_{serial_number}"

    while os.path.exists(new_folder_name):
        serial_number += 1
        new_folder_name = f"{base_folder_name}_{serial_number}"

    os.makedirs(new_folder_name)
    print(f"文件夹 '{new_folder_name}' 创建成功。")
    return new_folder_name

# 删除旧的 C28 和 M0 文件夹（如果存在）
delete_folder("C28")
delete_folder("M0")

# 创建新的 C28_X 和 M0_X 文件夹，其中 X 是流水号
create_folder_with_serial("C28")
create_folder_with_serial("M0")
