sudo find . | cpio -o -H newc | gzip > llimage.img
sudo cp llimage.img /boot/llimage.img
