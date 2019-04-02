//
//  hash.hpp
//  paper
//
//  Created by 朱瑞 on 19/3/23.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#ifndef hash_hpp
#define hash_hpp

#include <stdio.h>
#include<opencv2/opencv.hpp>
#include "wavelet.hpp"

class Hash{
private:
    std::string outlinehash_row(cv::Mat src,int num);
    std::string outlinehash_col(cv::Mat src,int num);
    std::string average_hash(cv::Mat &img,int r,int c);
     std::string block_hash(cv::Mat &img,int r,int c);
    std::string outline_hash(cv::Mat &img,int r,int c);
    std::string difference_hash(cv::Mat &img,int r,int c);
    std::string median_hash(cv::Mat &img,int r,int c);
    std::string perceptual_hash(cv::Mat &img,int r,int c);
    std::string wavelet_hash(cv::Mat &img,int r,int c);
    
    float HanmingDistance(std::string &str1,std::string &str2,int len);
    float OutlineDistance(std::string &str1,std::string &str2,int len);
    
public:
    std::string get_average_hash(cv::Mat &img,int r,int c);
    std::string get_outline_hash(cv::Mat &img,int r,int c);
    
    
    float Ans_average_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
    float Ans_block_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
    float Ans_difference_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
    float Ans_outline_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
    float Ans_median_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
    float Ans_perceptual_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
     float Ans_wavelet_hash(cv::Mat &img1,cv::Mat &img2,int r,int c);
};


#endif /* hash_hpp */
