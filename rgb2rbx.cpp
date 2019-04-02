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
    for (int j=0; j<nr-angle;j++)
        for (int i=0; i<nc-angle; i++){
            findrect(img, dest, j, i,angle,gap);
        }
    cv::imwrite(imgout11+"imgling3.png", dest);
}

bool findangle(cv::Mat &img,bool lefttop,int long_edge,int short_edge,int r,int c){
    if(lefttop){
        int gap=0;
        for(int i=0;i<long_edge;i++){
            if(img.at<uchar>(r,c+i)<200)
                gap++;
        }
        for(int i=0;i<short_edge;i++){
            if(img.at<uchar>(r+i,c)<200)
                gap++;
        }
        if(gap>5)return false;
        else return true;
        
    }else{
        int gap=0;
        for(int i=0;i<long_edge;i++){
            if(img.at<uchar>(r,c-i)<200)
                gap++;
        }
        for(int i=0;i<short_edge;i++){
            if(img.at<uchar>(r-i,c)<200)
                gap++;
        }
        if(gap>5)return false;
        else return true;
    
    }
}


cv::Mat findAllrect(cv::Mat &img,int long_angel,int short_angel){
    cv::Mat dest=cv::Mat::zeros(img.size(), CV_8UC1);
//    cv::Mat dest(img.size(),CV_8UC1);
//    cv::dilate(img, img, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
//    cv::imwrite(imgout11+"beforeRect.png", img);
    int nr= img.rows;
    int nc= img.cols ;
    std::vector<cv::Point2i> pleft,pright;
    cv::Point2i ptemp;
    for (int j=0; j<nr-short_angel;j++)
        for (int i=0; i<nc-long_angel; i++){
            if(findangle(img, true, long_angel, short_angel, j, i)){
                ptemp.x=j;
                ptemp.y=i;
                pleft.push_back(ptemp);
            }
        }
    for (int j=short_angel; j<nr;j++)
        for (int i=long_angel; i<nc; i++){
            if(findangle(img, false, long_angel, short_angel, j, i)){
                ptemp.x=j;
                ptemp.y=i;
                pright.push_back(ptemp);
            }
        }
    for(int i=0;i<pleft.size();i++){
        for(int j=0;j<pright.size();j++){
            int lr=pleft[i].x,lc=pleft[i].y,rr=pright[j].x,rc=pright[j].y;
            int total=2*(rr-lr+rc-lc),sumrect=0;
            for(int k=lr;k<=rr;k++){
                if(img.at<uchar>(k,lc)>200)sumrect++;
                if(img.at<uchar>(k,rc)>200)sumrect++;
            }
            for(int k=lc;k<=rc;k++){
                if(img.at<uchar>(lr,k)>200)sumrect++;
                if(img.at<uchar>(rr,k)>200)sumrect++;
            }
            float ans=(float)sumrect/(float)total;
            if(ans>0.93){
                //find rect
                for(int m=lr;m<=rr;m++){
                    dest.at<uchar>(m,lc)=255;
                    dest.at<uchar>(m,rc)=255;
                }
                for(int m=lc;m<=rc;m++){
                    dest.at<uchar>(lr,m)=255;
                    dest.at<uchar>(rr,m)=255;
                }
            }
        }
    }
    
    cv::imwrite(imgout11+"imgling44D.png", dest);
    return dest;
}


bool cmp1(std::vector<cv::Point> a,std::vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}

cv::Mat findline(cv::Mat &img,int lenHigh,int lenWidth,int miss){
    cv::Mat dest=cv::Mat::zeros(img.size(), CV_8UC1);
//    cv::Mat dest(img.size(),CV_8UC1);
    int nr= img.rows;
    int nc= img.cols ;
    for (int j=0; j<nr;j++) {
        for (int i=0; i<nc; i+=2) {
            int leng=i+lenWidth;
            if (leng>=nc){
                break;
            }
            int sum=0;
            for(int k=i;k<leng;k++){
                if(img.at<uchar>(j,k)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=0;k<nc;k++){
                    dest.at<uchar>(j,k)=255;
                }
                break;
            }
//            i=leng;
        }//for
    }//for
    //stright line
    for (int i=0; i<nc;i++) {
        for (int j=0; j<nr; j+=2) {
            int leng=j+lenHigh;
            if (leng>=nr){
                break;
            }
            int sum=0;
            for(int k=j;k<leng;k++){
                if(img.at<uchar>(k,i)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=0;k<nr;k++){
                    dest.at<uchar>(k,i)=255;
                }
                break;
            }
//            j=leng;
            
        }//for
    }//for
//    cv::imwrite(imgout11+"imgdest.png", dest);

    cv::dilate(dest, dest, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::Mat ans=cv::Mat::zeros(img.size(),img.type());
    img.copyTo(ans, dest);
//    cv::imwrite(imgout11+"imgans1.png", ans);

    std::vector<std::vector<cv::Point>> contours_small;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat mark=cv::Mat::zeros(img.size(),img.type());
    ans.copyTo(mark);
//    cv::imwrite(imgout11+"imgmark.png", mark);
    cv::findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask = cv::Mat ::zeros(img.size(),CV_8U);
    resultMask.setTo(0);
    cv::Mat resultImage = cv::Mat ::zeros(img.size(),CV_8U);
    
    std::sort(contours.begin(),contours.end(),cmp1);
    
    for(int i=0;i<contours.size();i++)
    {
        std::vector<std::vector<cv::Point>>::iterator it=contours.begin()+i;
        if(cv::contourArea(contours[i])<200 )
        {
            contours.erase(it);
            i--;
        }
    }
//    cv::imwrite(imgout11+"imgmark2.png", mark);
    drawContours(resultMask, contours, -1, cv::Scalar(255),CV_FILLED);
//    cv::imwrite(imgout11+"imgresultMask.png", resultMask);
    cv::Mat ans2=cv::Mat::zeros(img.size(),img.type());
    ans.copyTo(ans2, resultMask);
    
    return ans2;
//    cv::imwrite(imgout11+"imgling2.png", ans);
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

std::string hash_img(){
    std::string res;
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
    res=Ar+Ac+B1r+B1c+B2r+B2c+B3r+B3c+B4r+B4c+C1r+C1c+C2r+C2c+C3r+C3c+C4r+C4c;
    return res;
    
}
