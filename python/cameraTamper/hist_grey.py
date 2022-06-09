import os
import cv2
import pdb  
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
from io import BytesIO

def a_img_opencv():
    src = cv2.imread("./term2/3.jpg")
    print(src.shape)
    src = cv2.resize(src, (900,1440))
    plt.hist(src.ravel(), 256)
    plt.show()

def a_img_pil():
    image = Image.open("./term2/3.jpg")
    image = np.asarray(image)

    plt.hist(image.ravel(), 256)
    plt.show()

def normalization(data):
    _range = np.max(data) - np.min(data)
    return (data - np.min(data)) / _range

def cv2_grey(img):
    return cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

def cv2_ycrcb(img, y=True, cr=False, cb=False):
    imgYcc = cv2.cvtColor(img, cv2.COLOR_BGR2YCR_CB)
    (ymat,crmat,cbmat)= cv2.split(imgYcc)
    if y:
        return ymat
    if cr:
        return crmat
    if cb:
        return cbmat

def handle_video():
    cap = cv2.VideoCapture("video1.mp4")  #打开视频
    index = 0
    plt.ion()
    while (1):
        ret, frame = cap.read()  # 读取一帧视频
        # ret 读取了数据就返回True,没有读取数据(已到尾部)就返回False
        # frame 返回读取的视频数据--一帧数据
        index += 1
        
        if ret:
            print(index)
            cv2.imshow("capture", frame)  # 显示视频帧
            frame = cv2_grey(frame)
            histb = cv2.calcHist([frame], [0], None, [256], [0,255])

            histb = normalization(histb)
            plt.plot(histb, color='b')
            plt.show()
            plt.pause(0.02)
            plt.clf()  #清除图像

        if cv2.waitKey(40) & 0xFF == ord('q'):  # 等候40ms,播放下一帧，或者按q键退出
            break
    cap.release()  # 释放视频流
    cv2.destroyAllWindows() # 关闭所有窗口

def plt_2_image(src):
    # src = cv2.imread("./term2/3.jpg")
    # print(src.shape)
    # src = cv2.resize(src, (900,1440))
    if len(src.shape)>2 and src.shape[2]==3:
        print("bgr file")
        src = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)

    buffer_ = BytesIO(src)
    plt.hist(src.ravel(), 256)
    plt.savefig(buffer_,format = 'jpg')
    # buffer_.seek(0)
    dataPIL = Image.open(buffer_)
    dataPIL = dataPIL.convert("L")
    data = np.asarray(dataPIL)
    # plt.show()
    return data

def fusion_imgs():
    cap = cv2.VideoCapture("parking_full_cover.avi")
    index = 0
    while (1):
        ret, frame = cap.read()
        index += 1
        if ret and index%2==0:
            print(index)
            print("frame shape = ", frame.shape)
            frame = cv2.cvtColor(frame,cv2.COLOR_RGB2GRAY)
            image = plt_2_image(frame)
            print("histogram size = ", image.shape)

            imgs = np.hstack([frame,image])
            cv2.imshow("capture", imgs)  # 显示视频帧
        if cv2.waitKey(40) & 0xFF == ord('q'):  # 等候40ms,播放下一帧，或者按q键退出
            break

    cap.release()  # 释放视频流
    cv2.destroyAllWindows() # 关闭所有窗口

def listfile(path, device_id, ext=".jpg"):
    list_name = []
    for file in os.listdir(path):  
        file_path = os.path.join(path, file)
        if os.path.splitext(file)[1]==ext and device_id in file_path:
            list_name.append(file_path)

    list_name.sort()    
    return list_name

def accume_img(accu, newer):
    assert accu.shape == newer.shape

    alpha = 0.6
    out = accu * alpha + newer*(1 - alpha)
    assert out.shape == newer.shape

    return out.astype(newer.dtype)

## 阈值化做截断
def img_thre_bin(img, thres):
    _, mask = cv2.threshold(img, thres, 255, cv2.THRESH_BINARY)
    return mask

## 异或，找不同处
def xor_imgs(accimg, newimg, thresa):
    taccimg = img_thre_bin(accimg, thresa)
    tnewimg = img_thre_bin(newimg, thresa)

    bitwiseXor = cv2.bitwise_xor(taccimg, tnewimg)
    return bitwiseXor

## 异或，找不同处
def and_imgs(accimg, newimg, thresa):
    taccimg = img_thre_bin(accimg, min(thresa+20, 255))
    tnewimg = img_thre_bin(newimg, thresa)

    bitwiseand = cv2.bitwise_and(taccimg, tnewimg)
    return bitwiseand  

def handle_folder(fname, device_id, save_folder, acc_img):
    fpath = os.path.join("./", fname)
    imgs = listfile(fpath, device_id, ".png")
    
    # pdb.set_trace()
    # print(imgs)
    # plt.ion()
    # realname = ""
    print(device_id)
    for img in imgs:
        # realname = img
        
        frame = cv2.imread(img)
        frame = cv2.resize(frame, (640,480), interpolation=cv2.INTER_CUBIC)
        ## 可能有顺序问题，希望第一次不按权值累加，从第二次开始，处理完之后按权值累积
        if acc_img is None:
            acc_img = frame.copy()

        # thr_bin_img = img_thre_bin(acc_img, 132)
        # showimgs0 = np.hstack([acc_img,thr_bin_img])## 左边是累积原图，右边是累积阈值化的图
        
        res_xor = and_imgs(acc_img, frame, 128)
        # showimgs1 = np.hstack([frame, res_xor])## 左边是“异或”或者“和”的结果图，右边是当前帧
        
        # showimgs = np.vstack([showimgs1,showimgs0])
        # showimgs = np.hstack([frame, acc_img])

        thr_bin_frame = img_thre_bin(frame, 128)
        ratio_res = res_xor.sum()/thr_bin_frame.sum()

        show_ratio = float('%.2f' % ratio_res)
        
        if show_ratio < 0.89:
            # print(img)
            print(show_ratio)
        # cv2.imshow(device_id, showimgs)  # 显示视频帧
        # frame = cv2_grey(frame)

        if acc_img is not None:
            acc_img = accume_img(acc_img, frame)
        # frame = cv2_ycrcb(frame, y=True, cr=False, cb=False)
        # histb = cv2.calcHist([frame], [0], None, [100], [0,99])

        # histb = normalization(histb)
        # print(type(histb))
        # plt.plot(histb, color='b')
        # plt.show()
        # plt.pause(0.3)
    
        # if cv2.waitKey(300) & 0xFF == ord('q'):  # 等候40ms,播放下一帧，或者按q键退出
            # break
    
    # _, imgname = os.path.split(realname)
    # imgname = os.path.join("./",save_folder,imgname[:32]+".jpg")
    # plt.savefig(imgname, bbox_inches='tight')
    # plt.clf()  #清除图像
    # cv2.destroyAllWindows() # 关闭所有窗口

def mkdir(dirs):
    if not os.path.exists(dirs):
        os.makedirs(dirs)

def read_txt(txt_file):
    with open(txt_file, "r") as f:
        data = f.readlines()
        return data

if __name__ == "__main__":
    device_ids = read_txt("device_id_pos.txt")
    # device_ids = [
        # "3a690614d78bd50c29759ea6ca8ee62e",
        # "2b1251430439237391eb9656fc3eda0c",
        # "0bb68e40dd54427c979c158fe1cabf8b",
        # "4b032c3248b9ff6ecc5d4ac5600a8113",
        # "6c9375f71f618ce33d3d0a0393dfe1fd",
        # "6d7f8bf5d69eb270ea45dd30e769cbf0",
        # "9a7a6a2ef2058cf1021bc691f4687ddc",
        # "9aee7eeb8161c2b0239ce01aa8f86f38",
        # "15b77acc3d254069fcdc31cafc4ccab4",
        # "19dad00cb8f4c2a3d52726ee9f02a386"]
    
    for device_id in device_ids:
        accum_mat = None
        device_id = device_id.strip("\n")
        save_folder = "hist_dfi_pos"
        mkdir(os.path.join("./", save_folder))
        handle_folder("dfi_pos", device_id, save_folder, accum_mat)
    