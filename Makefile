
all: qmk_firmware build program

qmk_firmware:
	git clone https://github.com/qmk/qmk_firmware.git

build:
	rm -Rf qmk_firmware/keyboards/actg
	cp -Rf actg qmk_firmware/keyboards/
	cd qmk_firmware; make actg

program:
# pro micro
# note: you have to manually connect RESET and GND a few times before running this
#       and it might only rarely work.
	avrdude -v -patmega32u4 -cavr109 -P /dev/ttyACM* -b57600 -D \
	        -Uflash:w:qmk_firmware/.build/actg_default.hex:i

clean:
	rm -Rf qmk_firmware/.build
