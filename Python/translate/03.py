import openai
openai.api_key = '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'

from openai import OpenAI

client = OpenAI()

stream = client.chat.completions.create(
    model="gpt-4",
    messages=[{"role": "user", "content": "Say this is a test"}],
    stream=True,
)
for chunk in stream:
    if chunk.choices[0].delta.content is not None:
        print(chunk.choices[0].delta.content, end="")