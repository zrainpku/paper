//
//  rgb2rbx.hpp
//  paper
//
//  Created by 朱瑞 on 19/3/1.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#ifndef rgb2rbx_hpp
#define rgb2rbx_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
void rgb2RedRbx(cv::Mat &image,std::string outpath);
void huffindline(cv::Mat &src);
void findline(cv::Mat &img,int lenhigh,int minlengthwidth,int misspot);
void findbigrect(cv::Mat &img,int angle,int gap);
std::string hash_img();

void findAllrect(cv::Mat &img,int long_angel,int short_angel);

#endif /* rgb2rbx_hpp */
