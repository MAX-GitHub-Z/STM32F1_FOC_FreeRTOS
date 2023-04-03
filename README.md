# STM32F1_FOC_FreeRTOS
学习在STM32F103上运行FreeRTOS的同时运行FOC算法驱动电机

## FOC_FreeRTOS_V0.02 --2023.4.2

加载了角度检测线程和串口接收线程，实现角度检测功能和串口接收数据的功能

![IMG_20230403_094108](https://raw.githubusercontent.com/MAX-GitHub-Z/PicGo/main/202304030942646.jpg)

![IMG_20230403_094131](https://raw.githubusercontent.com/MAX-GitHub-Z/PicGo/main/202304030943426.jpg)

![IMG_20230403_094119](https://raw.githubusercontent.com/MAX-GitHub-Z/PicGo/main/202304030943667.jpg)

在转动电机时，STM32通过IIC读出AS5600的值获取当前时刻的角度，并通过UASRT1打印出来：![Snipaste_2023-04-03_09-45-54](https://raw.githubusercontent.com/MAX-GitHub-Z/PicGo/main/202304030947488.png)

 串口接收功能只实现了接收到数据并将数据打印出来的功能，其他功能将在后续开发

## FOC_FreeRTOS_V0.02 --2023.4.3

加载了
