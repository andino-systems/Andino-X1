#/bash
dtc -@ -I dts -O dtb -o sc16is752-spi1.dtbo sc16is752-spi1.dts
cp ./sc16is752-spi1.dtbo /boot/overlays/
