read_usb_hid: read_usb_hid.c
	gcc -O read_usb_hid.c -o read_usb_hid
	strip read_usb_hid
static: read_usb_hid.c
	gcc -O -static read_usb_hid.c -o read_usb_hid
	strip read_usb_hid

clean:
	rm read_usb_hid

rpxc:
	rpxc-gcc -O read_usb_hid.c -o read_usb_hid
