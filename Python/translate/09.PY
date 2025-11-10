import openai
import os

# 從環境變量中獲取 API 金鑰
api_key = os.environ.get("OPENAI_API_KEY")

if not api_key:
    raise ValueError("未找到 OPENAI_API_KEY 環境變量。")

# 使用獲取的 API 金鑰初始化 OpenAI 客戶端
client = openai.Client(api_key=api_key)

# 使用客戶端進行聊天
chat_completion = client.ChatCompletion.create(
    model="gpt-3.5-turbo",
    messages=[
        {
            "role": "user",
            "content": "Say this is a test",
        }
    ]
)
