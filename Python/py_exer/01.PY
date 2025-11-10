from kivy.app import App
from kivy.core.text import LabelBase
from kivy.uix.button import Button

LabelBase.register(name='msjh', fn_regular='C:\Windows\Fonts\msjh.ttc')  # 確保路徑正確

class MyApp(App):
    def build(self):
        self.button = Button(text='嗨嗨!', font_name='msjh', pos=(100, 100), size_hint=(None, None))
        self.button.bind(on_press=self.on_button_press)
        return self.button
    
    def on_button_press(self, instance):
        instance.text = 'Button Pressed!'

if __name__ == '__main__':
    MyApp().run()

