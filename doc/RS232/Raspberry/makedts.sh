#/bash
dtc -@ -I dts -O dtb -o sc16is752-spi0.dtbo sc16is752-spi0.dts
cp ./sc16is752-spi0.dtbo /boot/overlays/
