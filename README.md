# Notes

- For STM32WB5MM-DK

- Update WB FUS & BLE Stack FW before use!
    - See: [here](https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html)  (download HTML + view in browser)

- Exposes one service at `00002222-8e22-4541-9d4c-21edae82ed19`
    - Service characteristics:
        - Continuous data stream: `00002222-8e22-4541-9d4c-21edae82ed19`
        - Button presses: `00002223-8e22-4541-9d4c-21edae82ed19`

- Debug + trace printing available via ST-Link's VCOM port.

- Sometimes hardware-resetting after flashing is required

- Receiver software: [here](https://github.com/ETH-PBL/BLELog/tree/schilkp/st_demo)

---

Other links:
https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html
