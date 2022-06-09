import os

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

def listfile(path, ext=".jpg"):
    list_name = []
    for file in os.listdir(path):  
        file_path = os.path.join(path, file)
        if os.path.splitext(file)[1]==ext:
            list_name.append(file_path)

    list_name.sort()   
    return list_name

def write_txt(str_words, fname, mod="a+"):
    with open(fname,mod) as f:
        f.write(str_words)
        f.write('\n')

def get_img_list():
    img_folder = "/mnt/data/strange_face/"

    total_numbers = 0
    all_folders = listdir(img_folder)
    for afolder in all_folders:
        folder_name = afolder.split("/")[-1]
        txt_pth = os.path.join("/home/hxw/code", folder_name+".txt")

        print("********** folder name: ", folder_name, " **********")
        print("txt file: ", txt_pth)

        imgs_afolder = listfile(afolder, ext=".jpg")
        cur_img = 0
        file = open(txt_pth, "w")
        for aimg in imgs_afolder:
            file.write(aimg)
            file.write('\n')
            cur_img += 1
        file.close()

        print("number of imgs: ", cur_img)
        total_numbers += cur_img
    
    print("total number of images: ", total_numbers)
        
if __name__== "__main__":
    get_img_list()

