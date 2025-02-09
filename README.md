# Rasberry-PI led-matrix voice control

The project allows you to control RGB LED displays using Raspberry Pi GPIO with voice control from google speech regognition. The project also allows you to control arduino attached led panels from serial port (`rasberry-pi->arduino`).

## Running the project

You can run the following command  to startup the project after installing the prequisites to test switching led-matrix images based on voice control. You can edit the displayed videos/images and keywords which launch them in the `settings.json`

```bash
python3 ./audio_updater.py
```

## Prequisites

```bash
pip3 install -r requirements.txt
```

### Setting up usb microphone for voice recognition

https://tutorials-raspberrypi.com/build-raspberry-pi-voice-control-for-home-automation/

```
sudo apt-get update
sudo apt-get install flac
```

Check that usb sound adapter is detected
```
lsusb 
arecord -l
```

Record mic audio and check if your voice is heard 

```
arecord -d 10 -f cd -t wav -D plughw:1,0 test.wav
aplay -f dat test.wav
```

### Fixing ALSA (if audio is not recorded)

https://askubuntu.com/questions/150851/how-do-i-select-a-default-sound-card-with-alsa

```
sudo geany /etc/asound.conf

defaults.pcm.card 1
defaults.ctl.card 1
```

you can also edit system wide alsa base conf (not tested)

```
sudo cat proc/asound/modules
=> 1 snd_usb_audio
```

```
sudo geany /etc/modprobe.d/alsa-base.conf
options snd_usb_audio index=0
```

### setting up rpi-rgb-led-matrix

Just follow tutorial at:
https://github.com/hzeller/rpi-rgb-led-matrix

You most likely want to edit and build `led-image-viewer` under `utils` 

```
sudo apt-get update
sudo apt-get install libgraphicsmagick++-dev libwebp-dev -y
make led-image-viewer

```

Run image viewer
```
sudo ./led-image-viewer PATH/GIF_OR_IMAGE_NAME_HERE --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20
```

#### Led image viewer examples

If you have built the project correctly you should be able to view the results on attached led matrices.

```bash

sudo ./demo -D 0 --led-rows=64 --led-cols=32 --led-chain=2 --led-gpio-mapping=adafruit-hat
sudo ./led-image-viewer cat.gif --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="U-mapper"

sudo ./led-image-viewer images/colors.png --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat --led-chain=1

sudo ./video-viewer --led-rows=32 --led-cols=64 --led-chain=2 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20 images/SailorMoon-OP1.webm
sudo ./video-viewer --led-rows=32 --led-cols=64 --led-chain=1 --led-gpio-mapping=adafruit-hat-pwm --led-brightness 20 images/hxh.webm

```


### setting up project to run at startup

https://www.instructables.com/id/Raspberry-Pi-Launch-Python-script-on-startup/

```bash
chmod 755 start_rgb.sh
mkdir home/pi/logs
sudo crontab -e
@reboot /home/pi/led-matrix/start_rgb.sh >/home/pi/logs/cronlog_rgb 2>&11
```


### Setting up google speech recognition

by default you need to get google speech recognition credentials and add the  access key to a file named `service-speech.json`
```bash
pip3 install --upgrade oauth2client
pip3 install google-api-python-client
```



## Appendix

Debugging links:

https://bbs.archlinux.org/viewtopic.php?id=94696
https://github.com/Uberi/speech_recognition/issues/208
https://github.com/Uberi/speech_recognition/issues/199
https://thepi.io/how-to-use-your-raspberry-pi-as-a-wireless-access-point/
https://learn.sparkfun.com/tutorials/setting-up-a-raspberry-pi-3-as-an-access-point/all
