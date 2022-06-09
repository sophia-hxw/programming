#!/usr/bin/python3
#coding=utf-8
import cv2
import time

def cellMatch(mat1, mat2):
    """
    Input: 
        mat1    image1
        mat2    image2
    Output:
        numscal         value of (min_number_of_keypoints/max_number_of_keypoints)
        matchscal       value of (match_number_of_keypoints/min_number_of_keypoints) 
        kpimg1           key points of image1 
        kpimg2      key points of image2
        matches         match results, used by 
            cv2.drawMatches(img1_small, kpimg1, img2_small, kpimg2, matchnow, None)
            cv2.putText(img1_show, str(matchscale), (ib*w_crop_len, ia*h_crop_len+50), font, 1.5, (0, 0, 255), 3)
    """
    global orb
    global bf

    start_ts0 = int(time.time()*1000)
    kpimg1, desCat = orb.detectAndCompute(mat1,None)
    kpimg2, desSmallCat = orb.detectAndCompute(mat2, None)
    start_ts1 = int(time.time()*1000)

    feature_ts = start_ts1-start_ts0
    if len(kpimg1)==0 or len(kpimg2)==0:
        matchscal = 0
        matches = None
        match_ts = 0
    else:
        minnum = len(kpimg1) if(len(kpimg1)<len(kpimg2)) else len(kpimg2)

        start_ts2 = int(time.time()*1000)
        matches = bf.match(desCat, desSmallCat)
        start_ts3 = int(time.time()*1000)
        matchscal = len(matches)/minnum
        matchscal = float(format(matchscal,'.2f'))
        match_ts = start_ts3 - start_ts2

    return matchscal, kpimg1, kpimg2, matches, feature_ts , match_ts

def frameMatch(mat1, mat2):
    cell_scores = []
    f_times = []
    m_times = []
    match_score = 0
    
    """
    Input:
        img1
        img2
        crop_num            numbers which image split into in the direction of w and h
    Output:
        cell_scores         list, match scores of all cells in a image
    """
    crop_num = 5
    cell_match_score = 0.5
    if mat1 is None or mat2 is None:
        return match_score, cell_scores, f_times, m_times
    else:
        assert mat1.shape == mat2.shape
        
        w_crop_len = int(mat1.shape[1]/crop_num)
        h_crop_len = int(mat1.shape[0]/crop_num)

        match_num = 0
        for ia in range(crop_num): #h
            for ib in range(crop_num):#w
                img1_small = mat1[ia*h_crop_len:(ia+1)*h_crop_len, ib*w_crop_len:(ib+1)*w_crop_len]
                img2_small = mat2[ia*h_crop_len:(ia+1)*h_crop_len, ib*w_crop_len:(ib+1)*w_crop_len]
                matchscale, kpmat1, kpmat2, matchnow, feature_ts , match_ts = cellMatch(img1_small, img2_small)
                
                if matchscale>cell_match_score:
                    match_num += 1
                    
                cell_scores.append(matchscale)
                f_times.append(feature_ts)
                m_times.append(match_ts)

        match_score = match_num/(crop_num*crop_num)
        match_score = float(format(match_score,'.2f'))
        return match_score, cell_scores, f_times, m_times

if __name__ == '__main__':
    orb = cv2.ORB_create()
    bf = cv2.BFMatcher_create(cv2.NORM_HAMMING, crossCheck=True)

    img_before = request_body["img_before"]
    img_after = request_body["img_after"]
    cam_id = request_body["camera"]
    warn_ts = request_body["timestamp"]

    img1 = cv2.imread(img_before)
    img2 = cv2.imread(img_after)

    if img1 is None or img2 is None:
        print("img error")
    else:
        img1 = cv2.resize(img1,(1280,720))
        img2 = cv2.resize(img2,(1280,720))

        fram_thresholds = 0.38
        
        match_score, score_cells, feature_ts, match_ts = frameMatch(img1, img2)

        if match_score<fram_thresholds:
            print("get a warning")
