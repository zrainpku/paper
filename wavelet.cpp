//
//  wavelet.cpp
//  paper
//
//  Created by 朱瑞 on 19/3/28.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include "wavelet.hpp"


#include <math.h>
#include<stdlib.h>
#include <opencv2/opencv.hpp>
cv::Mat WaveTransform::WDT(const cv::Mat &_src,const std::string _wname,const int _level)
{
    cv::Mat src=cv::Mat_<float>(_src);
    cv::Mat dst=cv::Mat::zeros(src.rows,src.cols,src.type());
    int N=src.rows;
    int D=src.cols;
    //高通低通滤波器
    cv::Mat lowFilter;
    cv::Mat highFilter;
    wavelet(_wname,lowFilter,highFilter);
    //小波变换
    int t=1;
    int row=N;
    int col=D;
    while (t<=_level)
    {
        //先进行 行小波变换
        for (int i=0;i<row;i++)
        {
            //取出src中要处理的数据的一行
            cv::Mat oneRow=cv::Mat::zeros(1,col,src.type());
            for (int j=0;j<col;j++)
            {
                oneRow.at<float>(0,j)=src.at<float>(i,j);
            }
            oneRow=waveletDecompose(oneRow,lowFilter,highFilter);
            for (int j=0;j<col;j++)
            {
                dst.at<float>(i,j)=oneRow.at<float>(0,j);
            }
        }
//        char s[10];
//        std::itoa(t,s,10);
//        cv::imshow(s,dst);
//        waitKey();
//#if 0
//        //    normalize(dst,dst,0,255,NORM_MINMAX);
//        IplImage dstImg1=IplImage(dst);
//        cvSaveImage("dst1.jpg",&dstImg1);
//#endif
//        cv::normalize(dst,dst,0,255,cv::NORM_MINMAX);
        cv::imwrite("/Users/zrain/Desktop/scshot/hash/wavelet_hang.png", dst);
        //小波列变换
        for (int j=0;j<col;j++)
        {
            cv::Mat oneCol=cv::Mat::zeros(row,1,src.type());
            for (int i=0;i<row;i++)
            {
                oneCol.at<float>(i,0)=dst.at<float>(i,j);//dst,not src
            }
            oneCol=(waveletDecompose(oneCol.t(),lowFilter,highFilter)).t();
            for (int i=0;i<row;i++)
            {
                dst.at<float>(i,j)=oneCol.at<float>(i,0);
            }
        }
//#if 0
//        //    normalize(dst,dst,0,255,NORM_MINMAX);
//        IplImage dstImg2=IplImage(dst);
//        cvSaveImage("dst2.jpg",&dstImg2);
//#endif
//        cv::normalize(dst,dst,0,255,cv::NORM_MINMAX);
        cv::imwrite("/Users/zrain/Desktop/scshot/hash/wavelet_lie.png", dst);
        
        //更新
        row/=2;
        col/=2;
        t++;
        src=dst;
        
    }
    return dst;
}

//生成不同类型的小波
void WaveTransform::wavelet( const std::string _wname, cv::Mat &_lowFilter, cv::Mat &_highFilter )
{
    
    if (_wname=="haar" || _wname=="db1")
    {
        int N=2;
        _lowFilter=cv::Mat::zeros(1,N,CV_32F);
        _highFilter=cv::Mat::zeros(1,N,CV_32F);
        
        _lowFilter.at<float>(0,0)=1/sqrtf(N);
        _lowFilter.at<float>(0,1)=1/sqrtf(N);
        
        _highFilter.at<float>(0,0)=-1/sqrtf(N);
        _highFilter.at<float>(0,1)=1/sqrtf(N);
    }
    if (_wname=="sym2")
    {
        int N=4;
        float h[]={-0.483, 0.836, -0.224, -0.129};
        float l[]={-0.129, 0.224,    0.837, 0.483};
        
        _lowFilter=cv::Mat::zeros(1,N,CV_32F);
        _highFilter=cv::Mat::zeros(1,N,CV_32F);
        
        for (int i=0;i<N;i++)
        {
            _lowFilter.at<float>(0,i)=l[i];
            _highFilter.at<float>(0,i)=h[i];
        }
    }
    
}

//小波分解
cv::Mat WaveTransform::waveletDecompose( const cv::Mat &_src, const cv::Mat &_lowFilter, const cv::Mat &_highFilter )
{
    assert(_src.rows==1 && _lowFilter.rows==1 && _highFilter.rows ==1);
    assert(_src.cols>=_lowFilter.cols && _src.cols>=_highFilter.cols );
    cv::Mat src=cv::Mat_<float>(_src);
    
    int D=src.cols;
    
    cv::Mat lowFilter=cv::Mat_<float>(_lowFilter);
    cv::Mat highFilter=cv::Mat_<float>(_highFilter);
    
    //频域滤波或时域卷积；ifft( fft(x) * fft(filter)) = cov(x,filter)
    cv::Mat dst1=cv::Mat::zeros(1,D,src.type());
    cv::Mat dst2=cv::Mat::zeros(1,D,src.type());
    
    cv::filter2D(src,dst1,-1,lowFilter);
    cv::filter2D(src,dst2,-1,highFilter);
    
    //下采样
    cv::Mat downDst1=cv::Mat::zeros(1,D/2,src.type());
    cv::Mat downDst2=cv::Mat::zeros(1,D/2,src.type());
    
    cv::resize(dst1,downDst1,downDst1.size());
    cv::resize(dst2,downDst2,downDst2.size());
    
    //数据拼接
    for (int i=0;i<D/2;i++)
    {
        src.at<float>(0,i)=downDst1.at<float>(0,i);
        src.at<float>(0,i+D/2)=downDst2.at<float>(0,i);
        
    }
    return src;
}

