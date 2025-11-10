import openai

# 設置您的 API 金鑰
openai.api_key = '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'

# 使用 GPT 模型
response = openai.Completion.create(
  engine="text-davinci-003",
  prompt="這是一個問題的例子。",
  max_tokens=50
)

print(response.choices[0].text.strip())
