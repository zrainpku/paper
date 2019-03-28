//
//  wavelet.hpp
//  paper
//
//  Created by 朱瑞 on 19/3/28.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#ifndef wavelet_hpp
#define wavelet_hpp

#include <stdio.h>

#include<iostream>
#include <opencv2/opencv.hpp>
class WaveTransform{
private:
    void wavelet( const std::string _wname, cv::Mat &_lowFilter, cv::Mat &_highFilter );
    cv::Mat waveletDecompose( const cv::Mat &_src, const cv::Mat &_lowFilter, const cv::Mat &_highFilter );

public:
    cv::Mat WDT(const cv::Mat &_src,const std::string _wname,const int _level);

};

#endif /* wavelet_hpp */
