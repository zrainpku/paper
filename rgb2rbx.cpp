//
//  rgb2rbx.cpp
//  paper
//
//  Created by 朱瑞 on 19/3/1.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include "rgb2rbx.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
std::string imgout11="/Users/zrain/Desktop/scshot/temp/";
std::string img_pathhash="/Users/zrain/Desktop/scshot/hash/";

void rgb2RedRbx(cv::Mat &imag,std::string outpath) {
    cv::Mat image=imag.clone();
    int nr= image.rows;
    int nc= image.cols ;
    cv::Mat ans_float=cv::Mat(image.size(), CV_8UC3);
    for (int j=0; j<nr; j++) {
        for (int i=0; i<nc; i++) {
            
            int b=image.at<cv::Vec3b>(j,i)[0];
            int g=image.at<cv::Vec3b>(j,i)[1];
            int r=image.at<cv::Vec3b>(j,i)[2];
            int R,B,X;
            R=-r*1.4328+g*3.3769-1.9820*b+210.4212;
            B=-r*2.5135+g*5.9406-3.5089*b+168.6945;
            X=-2.4767*r+5.8416*g-3.4743*b+189.1942;
            
            R=(R>255?255:R);
            B=(B>255?255:B);
            X=(X>255?255:X);
            R=(R<0?0:R);
            B=(B<0?0:B);
            X=(X<0?0:X);
            
            image.at<cv::Vec3b>(j,i)[2]=R;
            image.at<cv::Vec3b>(j,i)[1]=B;
            image.at<cv::Vec3b>(j,i)[0]=X;
            
            
            
        }
    }
    
    imwrite(outpath, image);
}


void huffindline(cv::Mat &src){

    std::vector<cv::Vec4i>lines;
    cv::Mat destline(src.size(),CV_8U),imgsrc=src.clone();
    cv::HoughLinesP(imgsrc, lines, 1, CV_PI / 180, 80, 50, 10);
        
        for (size_t i = 0; i < lines.size(); i++)
        {
            cv::Vec4i l = lines[i];
            cv::line(destline, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(186, 88, 255), 1, cv::LINE_AA);
        }
    cv::imwrite(imgout11+"imgling.png", destline);
}

bool findrect(cv::Mat &src,cv::Mat &dest,int r,int c,int angle,int gap){
    for(int i=0;i<angle;i++){
        if(src.at<uchar>(r,c+i)>200 &&src.at<uchar>(r+i,c)>200)
            continue;
        else
            return false;
    }
    int nr=src.rows;
    int nc=src.cols;
    int right=0;
    int down=0;
    for(int i=c;i<nc-angle;){
        int num=0;
        for(int j=0;j<20;j++){
            if(src.at<uchar>(r,i+j)<200){
                num++;
            }
        }
        if(num<=gap){
            i+=5;
            continue;
        }
        else{
            right=i;
            i++;
            if(src.at<uchar>(r,i)<200)break;
        }
    }
    for(int i=r;i<nr-angle;){
        int num=0;
        for(int j=0;j<20;j++){
            if(src.at<uchar>(i+j,c)<200){
                num++;
            }
        }
        if(num<=gap){
            i+=5;
            continue;
        }
        else{
            down=i;
            i++;
            if(src.at<uchar>(i,c)<200)break;
        }
    }
    int num1=0;
    for(int j=r;j<r+50;j++){
        if(src.at<uchar>(j,right)<200){
            num1++;
        }
    }
    if( num1>12)return false;
    
    int num2=0;
    for(int j=c;j<c+50;j++){
        if(src.at<uchar>(down,j)<200){
            num2++;
        }
    }
    if(num1>12 || num2>12)return false;
    for(int i=c;i<=right;i++){
        dest.at<uchar>(r,i)=255;
        dest.at<uchar>(down,i)=255;
        }
    for(int i=r;i<=down;i++){
        dest.at<uchar>(i,c)=255;
        dest.at<uchar>(i,right)=255;
    }
    return true;
}

void findbigrect(cv::Mat &img,int angle,int gap){
    cv::Mat dest(img.size(),CV_8UC1);
    int nr= img.rows;
    int nc= img.cols ;
    for (int j=0; j<nr;j++)
        for (int i=0; i<nc; i++){
            findrect(img, dest, j, i,angle,gap);
        }
    cv::imwrite(imgout11+"imgling3.png", dest);
}


void findline(cv::Mat &img,int lenHigh,int lenWidth,int miss){
    cv::Mat dest(img.size(),CV_8UC1);
    int nr= img.rows;
    int nc= img.cols ;
    for (int j=0; j<nr;j++) {
        for (int i=0; i<nc; ) {
            int leng=i+lenWidth;
            if (leng>=nc){
                leng=nc;
            }
            int sum=0;
            for(int k=i;k<leng;k++){
                if(img.at<uchar>(j,k)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=i;k<leng;k++){
                    dest.at<uchar>(j,k)=255;
                }
            }
            i=leng;
            
            
        }//for
    }//for
    //stright line
    for (int j=0; j<nr;) {
        int leng=j+lenHigh;
        if (leng>=nr){
            leng=nr;
        }
        for (int i=0; i<nc; i++) {
            int sum=0;
            for(int k=j;k<leng;k++){
                if(img.at<uchar>(k,i)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=j;k<leng;k++){
                    dest.at<uchar>(k,i)=255;
                }
            }
            
            
            
        }//for
        j=leng;
    }//for
    cv::imwrite(imgout11+"imgling2.png", dest);
}

std::string hash_row(cv::Mat src,int num){
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

std::string hash_col(cv::Mat src,int num){
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

void hash_img(){
    cv::Mat src=cv::imread(img_pathhash+"src.png",CV_LOAD_IMAGE_GRAYSCALE);
    std::string Ar,Ac,B1r,B1c,B2r,B2c,B3r,B3c,B4r,B4c,C1r,C1c,C2r,C2c,C3r,C3c,C4r,C4c;
    std::ofstream outfile(img_pathhash+"hash.txt");
    cv::threshold(src, src, 40, 255,CV_THRESH_BINARY );
//    cv::imwrite(img_pathhash+"src1_255.png", src);
    cv::dilate(src, src, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
//    cv::imwrite(img_pathhash+"srcdilate.png", src);
    cv::resize(src, src, cv::Size(320, 180), (0, 0), (0, 0), cv::INTER_CUBIC);
    cv::threshold(src, src, 40, 255,CV_THRESH_BINARY );
    cv::imwrite(img_pathhash+"src160_902.png", src);
    Ar=hash_row(src,18);
    Ac=hash_col(src, 32);
    B1r=hash_row(src(cv::Rect(0,0,160,180)), 18);
    B1c=hash_col(src(cv::Rect(0,0,160,180)), 16);
    cv::imwrite(img_pathhash+"B1.png", src(cv::Rect(0,0,160,180)));
    B2r=hash_row(src(cv::Rect(160,0,160,180)), 18);
    B2c=hash_col(src(cv::Rect(160,0,160,180)), 16);
    cv::imwrite(img_pathhash+"B2.png", src(cv::Rect(160,0,160,180)));
    //heng xiang juli  bianwei 5==18
    B3r=hash_row(src(cv::Rect(0,0,320,90)), 18);
    B3c=hash_col(src(cv::Rect(0,0,320,90)), 32);
    cv::imwrite(img_pathhash+"B3.png", src(cv::Rect(0,0,320,90)));
    B4r=hash_row(src(cv::Rect(0,90,320,90)), 18);
    B4c=hash_col(src(cv::Rect(0,90,320,90)), 32);
    cv::imwrite(img_pathhash+"B4.png", src(cv::Rect(0,90,320,90)));
    C1r=hash_row(src(cv::Rect(0,0,160,90)), 18);
    C1c=hash_col(src(cv::Rect(0,0,160,90)), 16);
    cv::imwrite(img_pathhash+"C1.png", src(cv::Rect(0,0,160,90)));
    C2r=hash_row(src(cv::Rect(160,0,160,90)), 18);
    C2c=hash_col(src(cv::Rect(160,0,160,90)), 16);
    cv::imwrite(img_pathhash+"C2.png", src(cv::Rect(160,0,160,90)));
    C3r=hash_row(src(cv::Rect(0,90,160,90)), 18);
    C3c=hash_col(src(cv::Rect(0,90,160,90)), 16);
    cv::imwrite(img_pathhash+"C3.png", src(cv::Rect(0,90,160,90)));
    C4r=hash_row(src(cv::Rect(160,90,160,90)), 18);
    C4c=hash_col(src(cv::Rect(160,90,160,90)), 16);
    cv::imwrite(img_pathhash+"C4.png", src(cv::Rect(160,90,160,90)));
    outfile<<"Ar: "<<Ar<<std::endl;
    outfile<<"Ac: "<<Ac<<std::endl;
    outfile<<"B1r: "<<B1r<<std::endl;
    outfile<<"B1c: "<<B1c<<std::endl;
    outfile<<"B2r: "<<B2r<<std::endl;
    outfile<<"B2c: "<<B2c<<std::endl;
    outfile<<"B3r: "<<B3r<<std::endl;
    outfile<<"B3c: "<<B3c<<std::endl;
    outfile<<"B4r: "<<B4r<<std::endl;
    outfile<<"B4c: "<<B4c<<std::endl;
    outfile<<"C1r: "<<C1r<<std::endl;
    outfile<<"C1c: "<<C1c<<std::endl;
    outfile<<"C2r: "<<C2r<<std::endl;
    outfile<<"C2c: "<<C2c<<std::endl;
    outfile<<"C3r: "<<C3r<<std::endl;
    outfile<<"C3c: "<<C3c<<std::endl;
    outfile<<"C4r: "<<C4r<<std::endl;
    outfile<<"C4c: "<<C4c<<std::endl;
    outfile.close();
    
}
