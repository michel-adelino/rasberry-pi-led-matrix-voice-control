# Raspberry Pi LED Matrix Voice Control

This project enables voice-controlled operation of RGB LED displays using Raspberry Pi GPIO and Google Speech Recognition. Additionally, it allows LED panels attached to an Arduino to be controlled via a serial connection (`raspberry-pi -> arduino`).

## Running the Project

After installing the prerequisites, you can start the project by running the following command. This will enable switching LED matrix images based on voice control. You can configure the displayed videos/images and trigger keywords in `settings.json`.

```bash
python3 ./audio_updater.py
```

## Prerequisites

Install the required dependencies:

```bash
pip3 install -r requirements.txt
```

### Setting Up a USB Microphone for Voice Recognition

Follow this guide for setting up voice control:  
[Build Raspberry Pi Voice Control for Home Automation](https://tutorials-raspberrypi.com/build-raspberry-pi-voice-control-for-home-automation/)

Install necessary packages:

```bash
sudo apt-get update
sudo apt-get install flac
```

Check if the USB sound adapter is detected:

```bash
lsusb 
arecord -l
```

Test microphone recording and playback:

```bash
arecord -d 10 -f cd -t wav -D plughw:1,0 test.wav
aplay -f dat test.wav
```

### Fixing ALSA (if Audio is Not Recorded)

If audio is not recording, refer to this guide:  
[How to Select a Default Sound Card with ALSA](https://askubuntu.com/questions/150851/how-do-i-select-a-default-sound-card-with-alsa)

Edit ALSA configuration:

```bash
sudo geany /etc/asound.conf
```

Add the following lines:

```
defaults.pcm.card 1
defaults.ctl.card 1
```

Alternatively, modify the system-wide ALSA base configuration (not tested):

```bash
sudo cat /proc/asound/modules
# Output should be: 1 snd_usb_audio
```

```bash
sudo geany /etc/modprobe.d/alsa-base.conf
```

Add:

```
options snd_usb_audio index=0
```

### Setting Up `rpi-rgb-led-matrix`

Follow the installation guide:  
[rpi-rgb-led-matrix GitHub](https://github.com/hzeller/rpi-rgb-led-matrix)

You will likely need to edit and build `led-image-viewer` under the `utils` directory:

```bash
sudo apt-get update
sudo apt-get install libgraphicsmagick++-dev libwebp-dev -y
make led-image-viewer
```

Run the image viewer:

```bash
sudo ./led-image-viewer PATH/TO/YOUR_IMAGE_OR_GIF --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20
```

#### LED Image Viewer Examples

Once the setup is complete, you can display images and videos on the attached LED matrices:

```bash
sudo ./demo -D 0 --led-rows=64 --led-cols=32 --led-chain=2 --led-gpio-mapping=adafruit-hat

sudo ./led-image-viewer cat.gif --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="U-mapper"

sudo ./led-image-viewer images/colors.png --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat --led-chain=1

sudo ./video-viewer --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20 images/SailorMoon-OP1.webm

sudo ./video-viewer --led-rows=32 --led-cols=64 --led-chain=1 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20 images/hxh.webm
```

### Running the Project on Startup

To automatically run the project on startup, follow this guide:  
[Raspberry Pi - Launch Python Script on Startup](https://www.instructables.com/id/Raspberry-Pi-Launch-Python-script-on-startup/)

```bash
chmod 755 start_rgb.sh
mkdir -p /home/pi/logs
sudo crontab -e
```

Add the following line to the crontab:

```
@reboot /home/pi/led-matrix/start_rgb.sh >/home/pi/logs/cronlog_rgb 2>&1
```

### Setting Up Google Speech Recognition

By default, you need to obtain Google Speech Recognition credentials and store the access key in a file named `service-speech.json`.

Install the required libraries:

```bash
pip3 install --upgrade oauth2client
pip3 install google-api-python-client
```

## Appendix: Debugging Links

- [Arch Linux Forum - Audio Issues](https://bbs.archlinux.org/viewtopic.php?id=94696)
- [Speech Recognition GitHub Issue #208](https://github.com/Uberi/speech_recognition/issues/208)
- [Speech Recognition GitHub Issue #199](https://github.com/Uberi/speech_recognition/issues/199)
- [How to Use Your Raspberry Pi as a Wireless Access Point](https://thepi.io/how-to-use-your-raspberry-pi-as-a-wireless-access-point/)
- [Setting Up a Raspberry Pi 3 as an Access Point](https://learn.sparkfun.com/tutorials/setting-up-a-raspberry-pi-3-as-an-access-point/all)