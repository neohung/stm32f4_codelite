openocd  -f board/stm32f4discovery.cfg  -c "init" -c "reset halt" -c "flash protect 0 0 11 off" -c "flash erase_sector 0 0 11" -c "flash erase_check 0" -c "reset run" -c "exit"
