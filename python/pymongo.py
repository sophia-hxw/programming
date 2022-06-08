#!/usr/bin/python3
import pymongo

myclient = pymongo.MongoClient("mongodb://192.168.237.189:27017/")
mydb = myclient["testmongodb"]

mycol = mydb["sites"]
mydict = { "name": "RUNOOB", "alexa": "10000", "url": "https://www.runoob.com" }
x = mycol.insert_one(mydict) 
print("insert id: ",x.inserted_id)

dblist = myclient.list_database_names()
print("dblist name: ",dblist)
