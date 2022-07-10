# 基于stm32的谷歌小恐龙游戏（Google Chrome's Dino Game on STM32）

<img src="images\DinoGame.jpg">

## 元件清单 (components list)
- stm32f103c8t6
- OLED ssd1306 (128*64)
- 2个按键(2 button)

## 使用说明 (usage)

- 使用STM32CubeIDE导入工程 (Use STM32CubeIDE to import the project)
- IO连接关系
  - PB3 --- OLED SCK
  - PB4 --- OLED SDA
  - PB0 --- 开始/重新开始按键 (start/restart button)
  - PB1 --- 跳跃按键 (jump button)

## 其他
- 演示效果视频(Demo)：[B站/bilibili](https://www.bilibili.com/video/BV1234y1H75p/) 
- 原理讲解视频：
  - [手把手讲解实现小恐龙游戏-上集](https://www.bilibili.com/video/BV1bL4y1N77D/)
  - [手把手讲解实现小恐龙游戏-下集](https://www.bilibili.com/video/BV1CB4y1p7yh/)
