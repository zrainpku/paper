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


class Hash{
private:
    std::string outlinehash_row(cv::Mat src,int num);
    std::string outlinehash_col(cv::Mat src,int num);
    
public:
    double HanmingDistance(std::string &str1,std::string &str2,int len);
    std::string average_hash(cv::Mat &img,int r,int c);
    std::string outline_hash(cv::Mat &img,int r,int c);
};


#endif /* hash_hpp */
