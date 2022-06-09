import os
import cv2
import time
import random
import xml.etree.ElementTree as ET

## 列出path下所有子目录名称（带路径）
def listdir(path):
    list_name = []
    for file in os.listdir(path):  
        file_path = os.path.join(path, file)  
        if os.path.isdir(file_path): 
            list_name.append(file_path)
        else:  
            pass
    
    list_name.sort()
    return list_name

## 列出path下所有子文件名称（带路径）
def listimg(path):
    list_name = []
    for file in os.listdir(path):  
        file_path = os.path.join(path, file)  
        if not os.path.isdir(file_path): 
            list_name.append(file_path)
        else:  
            pass
    
    list_name.sort()
    return list_name

            
## 将文件夹层级结构减少，只保留最深处的两层，其中最深层为"res_1920X1080"，次深层为设备id
def first_step(path):
    path_list = listdir(path)
    for ia in range(len(path_list)):
        # 去掉文件夹名称中的空格，避免干扰mv命令
        rename = path_list[ia].replace(" ","\ ")
        newname = path_list[ia].replace(" ","_")
        commds0 = "mv " + rename + " "+newname
        print(commds0)
        os.system(commds0)
        path_list[ia] = newname
        print(path_list[ia])

    # for afolder in path_list:   
    #     # 去除文件夹层级结构 
    #     deep_pt = finddeepdir(afolder)
    #     print(deep_pt)
    #     commds = "mv " + deep_pt+ " ."
    #     print(commds)
    #     os.system(commds)

## 删除上一步中残留的空文件夹:"mnt {index}"，将图片移到名为设备id的文件夹中，删除"res_1920X1080"层级结构
def second_step():
    path_list = listdir("./")
    for folder in path_list:
        if len(folder)<10:
            cmds0 = "rm -rf "+folder
            print(cmds0)
            os.system(cmds0)
            continue
        cmds1 = "mv " + folder + "/res_1920x1080/* "+folder
        print(cmds1)
        os.system(cmds1)
        cmds2 = "rm -rf "+ folder + "/res_1920x1080"
        print(cmds2)
        os.system(cmds2)

## 一次随机取nums个数字
def get_n_imgs(length, nums):
    list_r = range(length)
    res = random.sample(list_r, nums)
    return res

## 每个摄像头任意挑选两张图片做测试
def get_deviceid_img(path, start_ind):
    folders = listdir(path)
    for afolder in folders:
        print("folder name: ", afolder)
        # 删除mac特有的隐藏文件夹DS_Store
        unusef = os.path.join(afolder, ".DS_Store")
        commd = "rm -rf "+unusef
        os.system(commd)

        ## 选择两张保留的图片
        files = listimg(afolder)
        if len(files)>2:
            inds = get_n_imgs(len(files), 2)
            print(inds)
            for ia in range(2):
                img_name = afolder[2:]+"_"+str(start_ind)+"_"+str(inds[ia])+".jpg"
                img_path = os.path.join("./", img_name)
                new_cmd = "mv "+files[inds[ia]]+" "+img_path
                # print(new_cmd)
                os.system(new_cmd)


# 原本目录结构为：天->小时->device_id->images，现改为device_id->小时->images
def reformat_folders(path):
    # path中是一天的数据
    outest_ = listdir(path)
    for fld in outest_:
        # fld是一个小时的所有摄像头数据
        outer_ = listdir(fld)
        for childfld in outer_:
            # childfld是设备id
            out_spt_pa = os.path.split(childfld)
            out_spt_ch = os.path.split(fld)
            out_new = os.path.join("./", out_spt_pa[-1],out_spt_ch[-1])
            mkdir(out_new)
            cmd_ = "mv "+childfld+"/* "+out_new+"/"
            print(cmd_)
            os.system(cmd_)

        cmd1_ = "rm -rf "+fld
        print(cmd1_)
        os.system(cmd1_)

## 取某个xml中的bbox并给同一个场景下的图片都生成同名的xml文件
def write_xml(xml_file, img_name_path):
    save_xml_name = img_name_path[:-3]+"xml"
    tree=ET.parse(xml_file)
    root = tree.getroot()

    name_node = root.find('filename')
    path_node = root.find('path')
    img_abs_path = os.path.abspath(img_name_path)
    name_node.text = os.path.split(img_name_path)[-1]
    path_node.text = img_abs_path

    # tree.write(save_xml_name, encoding="utf-8", xml_declaration=True)
    tree.write(save_xml_name, encoding="utf-8")

## 获取xml中的bbox的值
def get_annotation(xml_file, img_name_path):
    tree=ET.parse(xml_file)
    root = tree.getroot()

    for obj in root.iter('object'):
        xmlbox = obj.find('bndbox')
        b = (xmlbox.find('xmin').text, xmlbox.find('xmax').text, xmlbox.find('ymin').text, xmlbox.find('ymax').text)
        print(b)

## 在device_id->小时->images结构的数据中，每个divice_id随机选择800张图片用于测试
def get_deviceid_800imgs(path):
    print("folder name: ", path)
    folders = listdir(path) #列出当前device_id的按小时图片
    for afolder in folders: #一个小时的数据
        unusef = os.path.join(afolder, ".DS_Store")
        commd = "rm -rf "+unusef
        os.system(commd)

        ## 选择八百张保留的图片
        files = listfile(afolder, ext=".jpg")
        if len(files)>100:
            inds = get_n_imgs(len(files), 100)
            for ia in range(100):
                img_name = afolder[2:]+"_"+files[inds[ia]][-29:]
                img_path = os.path.join("./", img_name)
                new_cmd = "mv "+files[inds[ia]]+" "+img_path
                print(new_cmd)
                os.system(new_cmd)

## 将随机选择的每个divice_id的800张图片移动到同一个文件夹，同时去掉时间前缀
def move_to_imgs(path):
    files = listfile(path, ext=".jpg")
    for afile in files:
        cmds = "mv "+afile+" ./imgs/"+afile[2:-33]+"_"+afile[-29:]
        print(cmds)
        os.system(cmds)

## 视频文件转为图片文件
def handleVideo(video_name):
    folder_name = video_name[:-4]
    mkdir(folder_name)
   
    cap = cv2.VideoCapture(video_name)  #打开视频
    indexs = 0
    now_ind = 0
    NUM = cap.get(cv2.CAP_PROP_FRAME_COUNT)
    FPS = cap.get(cv2.CAP_PROP_FPS)
    print("all frames: ", NUM)
    print("fps: ", FPS)
    while (1):
        ret, frame = cap.read()  # 读取一帧视频
        
        if ret and now_ind==indexs:
            time_stmps = int(time.time()*1000)
            save_name = folder_name+"/image_"+str(time_stmps)+".jpg"
            print(save_name)
            cv2.imwrite(save_name, frame)  # 显示视频帧
            indexs += 50
        elif not ret:
            break
        else:
            now_ind += 1

        if cv2.waitKey(40) & 0xFF == ord('q'):  # 等候40ms,播放下一帧，或者按q键退出
            break
        
    cap.release()  # 释放视频流
    cv2.destroyAllWindows() # 关闭所有窗口

## device_id在文件名称中，获取后拼接到文件名中，并将文件移动到imgs文件夹
def id_folder_to_imgs():
    mkdir("./imgs")
    folders = listdir("./")
    for afold in folders:
        if "_" in afold:
            device_id = afold.split("_")[-1]
            all_files = listfile(afold, ext=".jpg")
            for afile in all_files:
                _,fname = os.path.split(afile)
                cmds = "cp "+afile+" ./imgs/"+device_id+"_"+fname
                print(cmds)
                os.system(cmds)

def write_txt(str_words, fname, mod="a+"):
    with open(fname,mod) as f:
        f.write(str_words)
        f.write('\n')

def ramdom_imgs(path, number):
    files = listfile(path, ext=".jpg")

    if len(files)>number:
        inds = get_n_imgs(len(files), number)
        for ia in range(number):
            _,fname = os.path.split(files[inds[ia]])
            write_txt(fname, "test_img_list_0908.txt")

            new_cmd = "cp "+files[inds[ia]]+" ./imgs/"
            print(new_cmd)
            os.system(new_cmd)

## 读文本文件
def read_txt(txt_file):
    with open(txt_file, "r") as f:
        data = [x.strip() for x in f.readlines()]
        return data

## 文件夹不存在就创建
def mkdir(dirs):
    if not os.path.exists(dirs):
        os.makedirs(dirs)

def rmdir(path, isDs_Store=True):
    if isDs_Store:
        cmds0 = "rm -rf "+os.path.join(path, ".DS_Store")
    os.system(cmds0)

## 返回path往下7层的子目录路径名称
def finddeepdir(path, layer_times=7):
    stat_time = 0
    while(True):
        rmdir(path, isDs_Store=True)
        file = os.listdir(path)
        file_path = os.path.join(path, file[0])
        
        stat_time += 1
        if not os.path.isdir(file_path): 
            return file_path
        elif stat_time>layer_times:
            return file_path
        else:
            path = file_path


## 列出path下所有后缀为ext的文件
def listfile(path, ext=".jpg"):
    list_name = []
    for file in os.listdir(path):  
        file_path = os.path.join(path, file)
        if os.path.splitext(file)[1]==ext:
            list_name.append(file_path)

    list_name.sort()   
    return list_name

def mv_imgs_list(flist, from_folder, to_folder):
    imgs = read_txt(flist)
    to_path = os.path.join("./", to_folder)
    for img in imgs:
        img=img.strip('\n')
        from_path = os.path.join("./",from_folder,img)
        cmds = "cp "+from_path+" "+to_path
        print(cmds)
        os.system(cmds)

def file_name(file_path):
    nearimgpth = []
    for root, _, files in os.walk(file_path):
        if len(files)>2:
            print(root) 
            nearimgpth.append(root)
    
    return nearimgpth


if __name__== "__main__":
    file_list = read_txt("./img_neg.lst")
    neg_file = read_txt("./res_neg.txt")

    for aneg in neg_file:
        aneg = aneg.split("    ")
        indx = int(aneg[0])
        print(indx)
        
        pth = file_list[indx]
        deviid = pth.split("/")[2]
        img_name = pth.split("/")[3]
        # print(deviid)
        # print(img_name)
        cmdcur = "cp " + pth + " ../negs_img/"+deviid+"_"+img_name
        print(cmdcur)
        os.system(cmdcur)


    # folders = listdir("./")
    # for folder in folders:
    #     subfiles = listfile(folder, ext=".gz")
    #     print("folder name: ", folder)
    #     print("number of subfiles: ", len(subfiles))
    #     for subfile in subfiles:
    #         cmdcur = "tar -zxvf "+subfile
    #         print(cmdcur)
    #         os.system(cmdcur)
    # os.system("rm -rf 0*")
    # os.system("rm -rf 1*")
    
    # need_handle_pth = file_name("./mnt")
    # print("how many folders needed to handle? :", len(need_handle_pth))
    # cur_root = "./"
    # for apth in need_handle_pth:
    #     deviceid = apth.split("/")[-2]
    #     # print(deviceid)
    #     cur_pth = os.path.join(cur_root,deviceid)
    #     if not os.path.exists(cur_pth):
    #         os.makedirs(cur_pth)
    #     cmdcur =  "cp "+apth+"/* "+cur_pth
    #     print(cmdcur)
    #     os.system(cmdcur)
    
    # folders = listdir("./imgs_neg") 
    # files = []
    # for afolder in folders:
    #     subfile = listfile(afolder, ext=".jpg")
    #     files.extend(subfile)
    # for file in files:
    #     write_txt(file,"img_neg.lst", mod="a+")

    #1，按deivice_id新建文件夹
    # did_from_txt = read_txt("./device_id_neg.txt")
    # print(did_from_txt)
    # for adid in did_from_txt:
    #     adid_pth = os.path.join("./", adid)
    #     mkdir(adid_pth)

    #1.1，去掉文件夹名称"mnt"和后面数字之间的空格
    # first_step("./18")
    #2，获取某个时段的文件的相应device_id
    # all_did = listdir("./10")
    # print(all_did)
    # for adid in all_did:
    #     did_path = finddeepdir(adid, layer_times=7)
    #     print("all path: ",did_path)
    #     did_name = did_path.split("/")[-1]
    #     print("device id: ", did_name)
    #     cmd1 = "mv "+did_path+"/* ./"+did_name
    #     print("command: ", cmd1)
    #     os.system(cmd1)

    ##3, 每间隔多少张选一张，生成txt列表
    # all_did = listdir("./")
    # tst_list = "test_15.txt"
    # sum_total = 0
    # for adid in all_did:
    #     rmdir(adid)
    #     imgs = listfile(adid, ext=".jpg")
    #     de_id = adid.split("/")[-1]
    #     print("********** device id :", de_id)
    #     print("img total numbers = ", len(imgs))
    #     sum_num = 0
    #     for ia in range(len(imgs)):
    #         if ia % 135 ==0:
    #             write_txt(imgs[ia], tst_list)
    #             sum_num += 1
    #     print("choosen img numbers = ", sum_num)
    #     sum_total += sum_num
    # print("--------------- total choosen img numbers: ", sum_total)


    
    # bigfolder = listdir("./")
    # for ias in range(len(bigfolder)):
    #     get_deviceid_img(bigfolder[ias], ias)
    
    # days = listdir("./")
    # for aday in days:
    #     reformat_folders(aday)


    # pngs = listfile("./", ext=".png")
    # xmls = listfile("./", ext=".xml")
    # for axml in xmls:
    #     print(axml)
    #     for apng in pngs:
    #         if os.path.split(axml)[-1].split("_")[0] in apng:
    #             write_xml(axml, apng)
            
    # device_ids = listdir("./")
    # for adevice in device_ids:
    #     get_deviceid_800imgs(adevice)       

    # device_ids = listdir("./")
    # for adevice in device_ids:
    #     move_to_imgs(adevice) 

    # videos = listfile("./", ext=".mp4")   ## 视频文件生成图片，每个视频文件一个文件夹
    # for avideo in videos:
    #     handleVideo(avideo)

    # id_folder_to_imgs()

    # ramdom_imgs("img_neg", 1200)

    # mv_imgs_list("neg.txt", "img_neg_all", "img_neg")


