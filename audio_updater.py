import speech_recognition as sr
import os
import sys
import json
import subprocess
import git
from dotenv import load_dotenv
import serial
import time
from pathlib import Path

current_directory = Path(__file__).parent
load_dotenv()

os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = f"{current_directory}/service-speech.json"
GIT_URL_WITH_PASS = os.getenv("GIT_URL_WITH_PASS")

with open('settings.json', 'r') as json_file:
   images_info = json.load(json_file)
    
   current_keyword = images_info['default']['id']
   reset_matrix = images_info['reset_matrix']['id']
   reboot_rasbi = images_info['reboot_rasbi']['id']
   shutdown_rasbi = images_info['shutdown_rasbi']['id']
   update_project = images_info['update_project']['id']
   
   mappings = images_info["mappings"]
   media_keys = list(images_info['media'].keys())
   images_configs = images_info['media']
   media_stripes = images_info['media-stripes']
   
def update_config():
   with open('settings.json', 'r') as json_file:
      images_info = json.load(json_file)
    
      current_keyword = images_info['default']['id']
      reset_matrix = images_info['reset_matrix']['id']
      reboot_rasbi = images_info['reboot_rasbi']['id']
      shutdown_rasbi = images_info['shutdown_rasbi']['id']
      update_project = images_info['update_project']['id']
   
      mappings = images_info["mappings"]
      media_keys = list(images_info['media'].keys())
      images_configs = images_info['media']
      media_stripes = images_info['media-stripes']
      

def stop_subprocess(subprocess):
    if subprocess is not None:
        subprocess.kill()
        
def listen_to_audio(m, r):
    with m as source:
        r.adjust_for_ambient_noise(source)
        print("Say something!")
        audio = r.listen(source)
    
        try:
            text = r.recognize_google_cloud(audio)
            print("Google Speech Recognition thinks you said " + text)
            return text
        except sr.UnknownValueError:
            print("Google Speech Recognition could not understand audio")
            return ''
        except sr.RequestError as e:
            print("Could not request results from Google Speech Recognition service; {0}".format(e))
        return ''

def match_least_one(match_all, match_candidates):
      
      is_match = False
      
      for word_list in match_all:
            is_match = True
            for word in word_list:
                  if word not in match_candidates:
                        is_match = False
            
            if is_match == True:
                  return True
      return False
      
      
def match_mappings(mappings, words):
      for mapping in mappings:
            #print("pairs")
            #print(mapping['word_pairs'])
            #print(words)
            if match_least_one(mapping['word_pairs'], words):
                  return mapping['id']
      return 'unidentified'
      
def update_repository():
      g = git.cmd.Git()
      g.pull(GIT_URL_WITH_PASS)
      
def reboot():
      os.system("sudo reboot")
      
def shutdown():
      os.system("sudo halt")
      
def serial_write(ser, message):
      ser.write(message.encode())
      time.sleep(2)
      
if __name__ == '__main__':
                
    # obtain audio from the microphone
    r = sr.Recognizer()
    m = sr.Microphone()
    
    image_process = subprocess.Popen(images_configs[current_keyword], shell=False)
    
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    time.sleep(5)
    serial_write(ser, '1')
    time.sleep(2)

    while True:
        try:
            words= listen_to_audio(m, r)
            word_list = words.lower().split()
            print(word_list)
            keyword = match_mappings(mappings, word_list)
            print(keyword)
            if keyword != current_keyword:
                  current_keyword = keyword
                  if keyword in media_keys:
                        stop_subprocess(image_process)
                        serial_write(ser, media_stripes[keyword])
                        image_process = subprocess.Popen(images_configs[keyword], shell=False)
                  elif keyword == reset_matrix:
                        serial_write(ser, '10')
                        stop_subprocess(image_process)
                  elif keyword == shutdown_rasbi:
                        serial_write(ser, '10')
                        stop_subprocess(image_process)
                        shutdown()
                  elif keyword == reboot_rasbi:
                        serial_write(ser, '10')
                        stop_subprocess(image_process)
                        reboot()
                  elif keyword == update_project:
                        update_repository()
                        update_config()
                  else:
                        print("unidentified keyword. do nothing")    
        
        except KeyboardInterrupt:
            stop_subprocess(image_process)
            serial_write(ser, '10')
            sys.exit(1)
    
    stop_subprocess(image_process)
