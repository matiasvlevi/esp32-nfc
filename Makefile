BOARD=esp32:esp32:esp32
CC=arduino-cli

PORT=/dev/ttyUSB0

build:
	$(CC) compile -b $(BOARD)

upload:
	$(CC) upload -p $(PORT) -b $(BOARD) ./


