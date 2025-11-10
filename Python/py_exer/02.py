from kivy.app import App
from kivy.core.text import LabelBase
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout

# 註冊中文字體（確保文件存在）
LabelBase.register(name="msjh", fn_regular="C:/Windows/Fonts/msjh.ttc")

class MyApp(App):
    def build(self):
        layout = BoxLayout(orientation="vertical")

        # 創建按鈕
        btn = Button(
            text="點我！",
            font_name="msjh",  # 使用自訂字體
            font_size=24,
            size_hint=(None, None),
            size=(200, 50)
        )

        # 按鈕事件
        def on_press(instance):
            print("按鈕被點擊！")

        btn.bind(on_press=on_press)

        layout.add_widget(btn)
        return layout

# 啟動應用
MyApp().run()
