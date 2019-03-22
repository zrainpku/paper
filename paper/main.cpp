//
//  main.cpp
//  paper
//
//  Created by 朱瑞 on 19/2/22.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include <iostream>
#include <string>
#include<opencv2/opencv.hpp>
#include "mytools.hpp"
#include "rgb2rbx.hpp"

std::string Websource="/Users/zrain/Desktop/scshot/";
std::string Webanswer="/Users/zrain/Desktop/scshot/";

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
//    WebSegment WS(Websource+"email.png");
//    std::string imgPath =WS.getOutImg();
//    std::cout<<"图片处理完毕,路径为:"<<imgPath<<std::endl;

    
    
    //huffbianhuan
//    cv::Mat img=cv::imread(Websource+"temp/resultfushi.png",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat img1=cv::imread(Websource+"temp/graytidu.png",CV_LOAD_IMAGE_GRAYSCALE);
//    huffindline(img);
//    findline(img, 50,100, 5);
//    findbigrect(img,40,10);
    
    //hash
    hash_img();

    return 0;
}
