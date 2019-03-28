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
#include "hash.hpp"

std::string Websource="/Users/zrain/Desktop/scshot/";
std::string Webanswer="/Users/zrain/Desktop/scshot/";

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
//    WebSegment WS(Websource+"picD.png");
//    std::string imgPath =WS.getOutImg();
//    std::cout<<"图片处理完毕,路径为:"<<imgPath<<std::endl;

    
    
    //huffbianhuan
//    cv::Mat img=cv::imread(Websource+"temp/resultImageD.png",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat img1=cv::imread(Websource+"temp/graytidu.png",CV_LOAD_IMAGE_GRAYSCALE);
//    huffindline(img);
//    findline(img, 50,100, 5);
//    findbigrect(img,40,10);
//    findAllrect(img, 100, 50);
    
    //hash
//    std::string test=hash_img();
    
    // class hash;
    Hash hash;
    cv::Mat img=cv::imread(Websource+"same/ppp.png",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img1=cv::imread(Websource+"same/picD.png",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img2=cv::imread(Websource+"same/picC.png",CV_LOAD_IMAGE_GRAYSCALE);
    
    cv::Mat img3=cv::imread(Websource+"temp/imgling44C.png",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img4=cv::imread(Websource+"temp/imgling44D.png",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat img4=cv::imread(Websource+"email.png",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::resize(img4, img4, cv::Size(320, 180), (0, 0), (0, 0), cv::INTER_CUBIC);
//    cv::imwrite(Websource+"email1.png", img4);
//    std::string ah1=hash.average_hash(img, 18, 32);
//    std::string ah2=hash.average_hash(img1, 18, 32);
//    std::cout<<"hash distance: "<<hash.HanmingDistance(ah1, ah2, 18*32)<<std::endl;
//    std::cout<<hash.Ans_average_hash(img, img1, 18, 32)<<std::endl;
    std::cout<<hash.Ans_wavelet_hash(img, img1, 18, 32)<<std::endl;
    
    
//    std::string oh1=hash.outline_hash(img2, 180, 320);
//    std::string oh2=hash.outline_hash(img3, 180, 320);
//    std::cout<<oh1<<std::endl;
//    std::cout<<oh2<<std::endl;
//    std::cout<<"hash distance: "<<hash.HanmingDistance(oh2, oh1, oh1.size())<<std::endl;

    return 0;
}
