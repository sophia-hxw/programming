# -*- coding: utf-8 -*-

import sys
import time
import json
import cv2
import base64
import numpy as np
import datetime
 
from kafka import KafkaConsumer
from kafka import TopicPartition
from kafka.errors import KafkaError
 
 
# KAFAKA_TOPIC = "cv2-logic-pidbysecond"
KAFAKA_TOPIC = "snap-cap"
 
consumer = KafkaConsumer(group_id="test", bootstrap_servers = '192.168.237.189:9092',enable_auto_commit = True,auto_commit_interval_ms = 5000)

consumer.assign([TopicPartition(topic=KAFAKA_TOPIC, partition=0)])

consumer.seek(TopicPartition(KAFAKA_TOPIC,0), 62063)

def base64_to_image(base64_code):
    img_data = base64.b64decode(base64_code)
    img_array = np.fromstring(img_data, np.uint8)
    img = cv2.imdecode(img_array, cv2.COLOR_RGB2BGR)
    return img

def oldCoding():
    total_num = 0
    for msg in consumer:
        jsonStr = bytes.decode(msg.value)
        msg_dict = json.loads(jsonStr)
        if msg_dict["regist"]==1:
            print("index: ",total_num)
            frame_dict = msg_dict["frame"]
            face_dict = msg_dict["face"]
            img_regis = base64_to_image(face_dict["grab_img"])
            img = img_regis[:,:,0:3]
            if img is not None:
                dtime = datetime.datetime.strptime(frame_dict["_timestamps"], "%Y-%m-%d %H:%M:%S")
                strdt = dtime.strftime("%Y_%m_%d_%H_%M_%S")
                filename = "./img/"+strdt+"_"+str(msg.offset)+".jpg"
                print("filename: ",filename)
                cv2.imwrite(filename,img)
            #print("time: ",frame_dict["_timestamps"])
            #print("offset: ",msg.offset)
            total_num += 1
        if total_num>500:
            break

    print("total numbers: ",total_num)

msg = next(consumer)
jsonStr = bytes.decode(msg.value)
msg_dict = json.loads(jsonStr)
print(msg_dict)
consumer.close()
