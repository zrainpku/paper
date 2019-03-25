//
//  mytools.cpp
//  paper
//
//  Created by 朱瑞 on 19/2/22.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include "mytools.hpp"
#include "frangi.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

std::string imgin="/Users/zrain/Desktop/scshot/";
std::string imgout="/Users/zrain/Desktop/scshot/temp/";

cv::Mat showWrinkleCommonLine(cv::Mat mat);

WebSegment::WebSegment(std::string imgPath){
    imgSrc=cv::imread(imgPath);
    imgDest=imgToRidge(imgSrc);
    imgDraw=showWrinkleCommonLine(imgSrc);
    cv::imwrite(imgout+"output.png", imgDraw);


}

WebSegment::~WebSegment(){
}

std::string WebSegment::getOutImg(){
    std::string res=imgout+"output.png";
    return res;
    
}

cv::Mat WebSegment::imgDeleteWords(cv::Mat &img){
    cv::Mat dest(img.size(),CV_8UC1);
    
    return dest;
}

cv::Mat WebSegment::imgToRidge(cv::Mat img){
    
    cv::Mat matSrc1=img.clone();
    //    cv::cvtColor(matSrc, matSrc, CV_BGR2GRAY);
    cv::Mat matDst;
//    std::vector<cv::Mat> labimg;
//    cv::split(matSrc1, labimg);
//    matSrc1=labimg[0];
    cv::imwrite(imgout+"gray0.png", matSrc1);
    cv::cvtColor(matSrc1, matDst, CV_BGR2GRAY);
    matSrc1=matDst;
    cv::imwrite(imgout+"gray1.png", matSrc1);
    
    int threshold = 0;
    float amount = 3;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc1, imgblurred, cv::Size(3, 3), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc1 - imgblurred)<threshold;
    matDst = matSrc1*(1 + amount) + imgblurred*(-amount);
    matSrc1.copyTo(matDst, lowcontrastmask);
    cv::imwrite(imgout+"grayblur.png", matDst);
    
    float alpha = 1.1;
    float beta = -16;
    for (int y = 0; y < matDst.rows; y++)
    {
        for (int x = 0; x < matDst.cols; x++)
        {
            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
        }
    }
    
    //        cv::equalizeHist(matDst, matDst);
    cv::imwrite(imgout+"grayruihua.png", matDst);
    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    cv::imwrite(imgout+"grayfushi.png", matDst);
    
    
    //end gray
    
    
    int scale = 1;
    int delta = 0;
    if( !matDst.data )
    {
        std::cout<<"load gray image error!"<<std::endl;
        return matDst;
    }
    
    cv::Mat grad,grad_temp;;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    
    cv::Sobel( matDst, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    cv::Sobel( matDst, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    
        grad.setTo(0);
        cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
        abs_grad_x.copyTo(grad);
        abs_grad_y.copyTo(grad,check);
    cv::imwrite(imgout+"graytidu00.png", grad);
//    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
//    cv::imwrite(imgout+"graytidu001.png", grad);
    
    
//    grad.convertTo(grad, CV_32FC1);
//    abs_grad_x.convertTo(abs_grad_x, CV_32FC1);
//    abs_grad_y.convertTo(abs_grad_y, CV_32FC1);
//    cv::Mat angle;
//    cv::cartToPolar(abs_grad_x, abs_grad_y, grad, angle);
//    cv::imwrite(imgout+"graytidu.png", grad);
    
    //end grident
    
    frangi2d_opts_t opt;
    opt.sigma_start=1;
    opt.sigma_end=3;
    opt.sigma_step=1;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    
    cv::Mat scales(grad.size(),CV_32FC1);
    cv::Mat directions(grad.size(),CV_32FC1);
    cv::Mat src(grad.size(),CV_32FC1);
    
    grad.convertTo(src, CV_32FC1);
    cv::Mat outfrangi;
    frangi2d(src,outfrangi,scales,directions,opt);
    outfrangi.convertTo(outfrangi, CV_8UC1);
    
    //end frangi
    cv::Mat out,out_x(grad.size(),CV_8U),out_y(grad.size(),CV_8U);
//    outfrangi.convertTo(outfrangi, CV_8UC1);
    cv::imwrite(imgout+"xfrange1.png", outfrangi);
    
    cv::threshold(outfrangi, out, 0, 255,CV_THRESH_BINARY);
    cv::imwrite(imgout+"xfrange2.png", out);
    //        cv::medianBlur(out, out, 3);
    
    //    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_outfrangi.bmp", out);
    
    return out;
    
    
}

bool cmp(std::vector<cv::Point> a,std::vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}

cv::Mat showWrinkleCommonLine(cv::Mat mat){
    cv::Mat src=mat.clone();
    cv::Mat mark=cv::imread(imgout+"xfrange2.png",CV_LOAD_IMAGE_GRAYSCALE) ;
    
    //****************************
    
    std::vector<std::vector<cv::Point>> contours_small;
    cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );
    
    //filter
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
        cv::morphologyEx(mark2,mark2,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::imwrite(imgout+"resultfushi.png", mark2);
    
    
    cv::findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask = cv::Mat ::zeros(mark.size(),CV_8U);
    resultMask.setTo(0);
    cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
    
    sort(contours.begin(),contours.end(),cmp);
    
    for(int i=0;i<contours.size();i++)
    {
        std::vector<std::vector<cv::Point>>::iterator it=contours.begin()+i;
//        cv::Point2f center;
//        float radius;
//        minEnclosingCircle(contours[i],center,radius);
//        float cirArea=3.1415926*radius*radius;
//        float div=contourArea(contours[i])/cirArea;
        
        if(cv::contourArea(contours[i])<300 )
        {
            contours.erase(it);
            i--;
        }
        
    }
    
//    contours_small.clear();
//    sort(contours.begin(),contours.end(),cmp);
//    for(int i=0;i<contours.size();i++)
//    {
//                if(i>10)break;
//        contours_small.push_back(contours[i]);
//        
//        
//    }
    
    drawContours(resultMask, contours, -1, cv::Scalar(255),CV_FILLED);
    //    cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);
    cv::Mat resultImage1;
    
    cv::imwrite(imgout+"resultImagemark.png", resultMask);
    
    mark.copyTo(resultImage, resultMask);
    
    //    cv::morphologyEx(resultImage,resultImage,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::imwrite(imgout+"resultImageD.png", resultImage);
    
    //    mark2=resultImage.clone();
    //    contours.clear();
    //    hierarchy.clear();
    //    contours_small.clear();
    //    resultMask.setTo(0);
    //    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    //    sort(contours.begin(),contours.end(),cmp);
    //    for(int i=0;i<contours.size();i++)
    //    {
    ////        if(i>60)break;
    //        contours_small.push_back(contours[i]);
    //
    //
    //    }
    //    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
    //    resultImage.copyTo(resultImage1, resultMask);
    //
    //
    //
    //    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_resultImage1.bmp", resultImage1);
    
    
    
    
    //*********************************
    //    cv::Mat mark3(resultImage.size(),CV_8UC1);
    //    mark3.setTo(0);
    //    vector<int> ans_area;
    //    vector<float> ans_len;
    
    //    int num=0;
    //    for(int r=resultImage.rows-1;r>=0;r--)
    //        for(int c=0;c<resultImage.cols;c++)
    //        {
    //            if((int)resultImage1.at<uchar>(r,c)>250 && (int)mark3.at<uchar>(r,c)==0)
    //            {
    //                num++;
    //                int area=0,len=0;
    //                int rr=r,cc=c;
    //                countWrinkle(resultImage1, mark3, r, c, num,area,len);
    //                //float tlen=sqrt((rr-r)*(rr-r)+(cc-c)*(cc-c));
    //                ans_len.push_back(len);
    //                ans_area.push_back(area);
    //
    //            }
    //
    //        }//for
    //
    //    sort(ans_len.begin(),ans_len.end());
    //    sort(ans_area.begin(),ans_area.end());
    //
    //    for(int i=0;i<ans_len.size();i++){
    //        cout<<"第"<<i<<"根皱纹： 长度"<<ans_len[i]<<"(像素)-----  面积："<<ans_area[i]<<"像素。"<<endl;
    //
    //    }
    
    
    
    
    
    //begin to draw
    for(int r=0;r<src.rows;r++)
        for(int c=0;c<src.cols;c++)
        {
            //            cout<<(int)mark.at<uchar>(r,c)<<"--";
            if((int)resultImage.at<uchar>(r,c)>225){
                src.at<cv::Vec3b>(r,c)[0]=25;
                src.at<cv::Vec3b>(r,c)[1]=25;
                src.at<cv::Vec3b>(r,c)[2]=255;
                
                
            }
            
        }
    
    return src;

}


