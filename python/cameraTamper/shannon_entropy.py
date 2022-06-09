from PIL import Image
import math
import numpy as np
import matplotlib.pyplot as plt

####### calculate the shannon entropy for an image ###############
def shannon_entropy_pil(img):

    histogram = img.histogram()
    histogram_length = sum(histogram)

    samples_probability = [float(h) / histogram_length for h in histogram]

    return -sum([p * math.log(p, 2) for p in samples_probability if p != 0])

def entropy_np(band: np.ndarray) -> np.float64:
    """
    Compute the entropy content of an image's band.
    :param band: The band data to analyze. np.asarray(band)
    :return:     Data entropy in bits.
    """
    hist, _ = np.histogram(np.asarray(band), bins=range(0, 256))
    hist = hist[hist > 0]
    probablity = hist / np.sum(hist)
    result = np.sum(np.matmul(probablity.T, np.log2(probablity)))
    return -result

####### calculate the shannon entropy for small slices of an image
def focalpoint(img):
    
    w = img.size[0]
    h = img.size[1]
    dims = [ w,h ]
    print(dims)
    
    slice_size = 100
    
    tiles = []
    max_ent = 0
    
    for y in range(0,h,slice_size):
        for x in range(0,w,slice_size):
            mx = min(x+slice_size, w)
            my = min(y+slice_size, h)

            buff = Image.new("RGB", [slice_size, slice_size], (255, 255, 255)) 
            tile = img.crop((x, y, mx, my))
            buff.paste(tile, (0, 0))

            ent = shannon_entropy_pil(buff)

            data = {'entropy':ent, 'x':x, 'y':y}

            if ent > max_ent:
                max_ent = ent
                tiles = data

    # if tiles['x'] > slice_size:
    #     tiles['x'] -= slice_size
        
    # if tiles['y'] > slice_size:
    #     tiles['y'] -= slice_size

    # if (tiles['x'] + crop_size) > w:
    #     tiles['x'] = w - crop_size

    # if (tiles['y'] + crop_size) > h:
    #     tiles['y'] = h - crop_size

    tiles['w'] = w
    tiles['h'] = h

    return tiles

####### calculate the shannon entropy for an image split into SLICE_NUM pieces
def slices_entropy(img, slic_x, slic_y):
    w = img.size[0]
    h = img.size[1]
    
    wstep = int(w/slic_x)
    hstep = int(h/slic_y)
    
    tiles = []

    for xa in range(slic_x):
        for ya in range(slic_y):
            y = ya*hstep
            x = xa*wstep
            my = min((ya+1)*hstep, h)
            mx = min((xa+1)*wstep, w)

            buff = Image.new("RGB", [wstep, hstep], (255, 255, 255)) 
            tile = img.crop((x, y, mx, my))
            buff.paste(tile, (0, 0))

            ent = shannon_entropy_pil(buff)
            # print((x, y, mx, my))
            # print(ent)
            data = {'entropy':ent, 'xmin':x, 'ymin':y, 'xmax':mx, 'ymax':my}
            tiles.append(data)

    return tiles

def pil_grey_entropy(image, slic_x, slic_y):
    grey = image.convert("L")
  
    en1 = slices_entropy(grey, slic_x, slic_y)
    grey_ent = ["%.2f"%(ia['entropy']) for ia in en1]

    return grey_ent

def pil_y_entropy(image, slic_x, slic_y):
    # rgb = im.convert("RGB")
    # hsv = im.convert("HSV")
    ycrcb = image.convert("YCbCr")
    # r, g, b = rgb.split()
    # h, s, v = hsv.split()
    # y, cr, cb = ycrcb.split()
    y, _, _ = ycrcb.split()

    en2 = slices_entropy(y, slic_x, slic_y)
    y_ent = ["%.2f"%(ia['entropy']) for ia in en2]

    return y_ent

def handleVideo():
    import cv2
    slic_x = 4
    slic_y = 4
    cap = cv2.VideoCapture("parking_full_cover.avi")  #打开视频
    while (1):
        ret, frame = cap.read()  # 读取一帧视频
        # ret 读取了数据就返回True,没有读取数据(已到尾部)就返回False
        # frame 返回读取的视频数据--一帧数据

        if ret:
            # print(frame.shape)
            image = Image.fromarray(cv2.cvtColor(frame,cv2.COLOR_BGR2RGB))
            image = pil_grey_entropy(image, slic_x, slic_y)
            image = cv2.cvtColor(np.asarray(image),cv2.COLOR_RGB2BGR)
            cv2.imshow("capture", image)  # 显示视频帧

        if cv2.waitKey(40) & 0xFF == ord('q'):  # 等候40ms,播放下一帧，或者按q键退出
            break
    cap.release()  # 释放视频流
    cv2.destroyAllWindows() # 关闭所有窗口

if __name__ == "__main__":   
    handleVideo()