dd if=/dev/zero of=~/test1.bin count=1000k
sudo losetup /dev/loop1 ~/test1.bin
