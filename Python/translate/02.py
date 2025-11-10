

import openai

# 設定 API 金鑰
openai.api_key = "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"

# 創建一個請求來使用 GPT 模型
response = openai.Completion.create(
  model="text-davinci-004",  # 指定使用的模型，例如 "text-davinci-003"
  prompt="翻譯以下句子到英文：'你好, 世界'",  # 您要模型回答的問題或進行的任務
  temperature=0.7,  # 設定創造性的程度，0.7 是一個中等值
  max_tokens=60  # 最大生成的字符數
)

# 輸出生成的文本
print(response.choices[0].text.strip())
