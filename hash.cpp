//
//  hash.cpp
//  paper
//
//  Created by 朱瑞 on 19/3/23.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include "hash.hpp"

double Hash::HanmingDistance(std::string &str1,std::string &str2,int len)
{
    if((str1.size()!=len)||(str2.size()!=len))
        return -1;
    double difference = 0;
    for(int i=0;i<len;i++)
    {
        if(str1[i]==str2[i])
            difference++;
    }
    return difference/len;
}




std::string Hash::average_hash(cv::Mat &img,int r,int c){
    std::string res;
    cv::Mat src=img.clone();
//    cv::threshold(src, src, 40, 255,CV_THRESH_BINARY );
//    cv::dilate(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7)));
    cv::resize(src, src, cv::Size(c, r), (0, 0), (0, 0), cv::INTER_CUBIC);
    cv::imwrite("/Users/zrain/Desktop/scshot/hash/average.png", src);
    int mean=0;
    for(int i=0;i<r;i++ ){
        for(int j=0;j<c;j++){
            mean+=src.at<uchar>(i,j);
        }
    }
    mean/=r*c;
    std::cout<<"mean: "<<mean<<std::endl;
    for(int i=0;i<r;i++ ){
        for(int j=0;j<c;j++){
//            std::cout<<"i: "<<i<<"j: "<<j<<(int)src.at<uchar>(i,j)<<std::endl;
            if((int)src.at<uchar>(i,j)>mean){
//                src.at<uchar>(i,j)=255;
                res+="1";
            }else{
//                src.at<uchar>(i,j)=0;
                res+="0";
            }
           
        }
    }
    
    return res;
}

std::string Hash::outline_hash(cv::Mat &img,int r,int c){
    std::string res;
    cv::Mat src=img.clone();
    std::string Ar,Ac,B1r,B1c,B2r,B2c,B3r,B3c,B4r,B4c,C1r,C1c,C2r,C2c,C3r,C3c,C4r,C4c;
    cv::threshold(src, src, 40, 255,CV_THRESH_BINARY );
    cv::dilate(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    //320 * 180
    cv::resize(src, src, cv::Size(c, r), (0, 0), (0, 0), cv::INTER_CUBIC);
    cv::threshold(src, src, 40, 255,CV_THRESH_BINARY );
    Ar=outlinehash_row(src,r/10);
    Ac=outlinehash_col(src, c/10);
    
    B1r=outlinehash_row(src(cv::Rect(0,0,c/2,r)), r/10);
    B1c=outlinehash_col(src(cv::Rect(0,0,c/2,r)), c/20);
    
    B2r=outlinehash_row(src(cv::Rect(c/2,0,c/2,r)), r/10);
    B2c=outlinehash_col(src(cv::Rect(c/2,0,c/2,r)), c/20);
    //heng xiang juli  bianwei 5==18
    B3r=outlinehash_row(src(cv::Rect(0,0,c,r/2)), r/10);
    B3c=outlinehash_col(src(cv::Rect(0,0,c,r/2)), c/10);

    B4r=outlinehash_row(src(cv::Rect(0,r/2,c,r/2)), r/10);
    B4c=outlinehash_col(src(cv::Rect(0,r/2,c,r/2)), c/10);

    C1r=outlinehash_row(src(cv::Rect(0,0,c/2,r/2)), r/10);
    C1c=outlinehash_col(src(cv::Rect(0,0,c/2,r/2)), c/20);

    C2r=outlinehash_row(src(cv::Rect(c/2,0,c/2,r/2)), r/10);
    C2c=outlinehash_col(src(cv::Rect(c/2,0,c/2,r/2)), c/20);

    C3r=outlinehash_row(src(cv::Rect(0,r/2,c/2,r/2)), r/10);
    C3c=outlinehash_col(src(cv::Rect(0,r/2,c/2,r/2)), c/20);

    C4r=outlinehash_row(src(cv::Rect(c/2,r/2,c/2,r/2)), r/10);
    C4c=outlinehash_col(src(cv::Rect(c/2,r/2,c/2,r/2)), c/20);
    res=Ar+Ac+B1r+B1c+B2r+B2c+B3r+B3c+B4r+B4c+C1r+C1c+C2r+C2c+C3r+C3c+C4r+C4c;
    return res;
}

std::string Hash::outlinehash_row(cv::Mat src,int num){
    std::string res;
    int nr=src.rows;
    int nc=src.cols;
    bool flag=false;
    int nums=0;
    int div=nr/num;
    for(int i=0;i<nr;i++){
        if(i%div==0 && i>0){
            if(flag == true){
                res+="1";
            }else{
                res+="0";
            }
            flag=false;
            
        }
        nums=0;
        for(int j=0;j<nc && flag==false;j++){
            if(src.at<uchar>(i,j)>100)nums++;
        }
        if(nums>(nc/2)){
            flag=true;
        }
    }
    if(flag == true){
        res+="1";
    }else{
        res+="0";
    }
    
    return res;
}

std::string Hash::outlinehash_col(cv::Mat src,int num){
    std::string res;
    int nr=src.rows;
    int nc=src.cols;
    bool flag=false;
    int nums=0;
    int div=nc/num;
    for(int i=0;i<nc;i++){
        if(i%div==0 && i>0){
            if(flag == true){
                res+="1";
            }else{
                res+="0";
            }
            flag=false;
            
        }
        nums=0;
        for(int j=0;j<nr && flag==false;j++){
            if(src.at<uchar>(j,i)>100)nums++;
        }
        //        std::cout<<i<<": "<<nums<<std::endl;
        if(nums>(nr/2)){
            flag=true;
        }
    }
    if(flag == true){
        res+="1";
    }else{
        res+="0";
    }
    
    return res;
}
