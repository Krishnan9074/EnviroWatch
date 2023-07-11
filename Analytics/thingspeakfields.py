
import openai
import gradio
import requests
response = requests.get("https://api.thingspeak.com/channels/2215231/feeds.json?api_key=7IVL736EXRIIX1LH&results=2")
print(response.content)
data = response.json()
data_dictionary = dict(data)
data1=data_dictionary["feeds"][1]["field1"]
data2=data_dictionary["feeds"][1]["field2"]
print(data_dictionary["feeds"][1]["field1"])
print(data_dictionary["feeds"][1]["field2"])
print(data_dictionary["feeds"][1]["field3"])
print(data_dictionary["feeds"][1]["created_at"])
openai.api_key = "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"

messages = [{"role": "system", "content": "You are a weather expert who suggests about clothing and products based on the temperature and humidity provided"}]
msg= f"temperature:{data1},humidity:{data2}"
def CustomChatGPT(user_input):
    messages.append({"role": "user", "content":msg})
    response = openai.ChatCompletion.create(
        model = "gpt-3.5-turbo",
        messages = messages
    )
    ChatGPT_reply = response["choices"][0]["message"]["content"]
    messages.append({"role": "assistant", "content": ChatGPT_reply})
    return ChatGPT_reply

demo = gradio.Interface(fn=CustomChatGPT, inputs = [], outputs = "text", title = "Envio-your weather companion(gets thingspeak data and predicts general clothing and products)")

demo.launch(share=True)