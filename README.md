# tuya-duck-keyboard
This project is developed using Tuya SDK, which enables you to quickly develop branded
apps connecting and controlling smart scenarios of many devices.
For more information, please check Tuya Developer Website.  

1. 芯片使用了stm32f042，固件移植QMK，硬件资料链接：https://lceda.cn/Doravmon/wen-shi-du-kuo-zhan-ban
2. 代码复制到QMK固件的keyboards目录下，使用qmk compile -kb tuya -km numkb编译
3. 使用涂鸦智能的wb3s模块，上传温湿度数据到云端和app
4. 移植涂鸦的SDK到QMK固件，下发命令到键盘，实现键盘自动输入

