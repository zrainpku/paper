//
//  mytools.hpp
//  paper
//
//  Created by 朱瑞 on 19/2/22.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#ifndef mytools_hpp
#define mytools_hpp

#include<opencv2/opencv.hpp>
#include<string>
class WebSegment {
private:
//    cv::Mat imgSrc;
//    cv::Mat imgDest;
//    cv::Mat imgDraw;
//    cv::Mat imgNoWords;
    cv::Mat imgToRidge(cv::Mat mat);
    cv::Mat imgDeleteWords(cv::Mat &mat);
    cv::Mat showWrinkleCommonLine(cv::Mat mat);
public:
    WebSegment();
    ~WebSegment();
    std::string getOutImg();
    cv::Mat getAnsImg(cv::Mat &img);
    


};//class

#endif /* mytools_hpp */
