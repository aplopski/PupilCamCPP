/*
This file is reimplimintation of the pyuvc scripts by Pupil-Labs 
https://github.com/pupil-labs/pyuvc

handles camera opening/closing in windows for libusbK drivers
handles camera frame grabs - return openCV frames


Copyright
Alexander Plopski
NAIST Interactive Media Design Laboratory
26.12.2016
*/

//#pragma once

#ifndef FRAMEGRABBER_H_
#define FRAMEGRABBER_H_

#include <Windows.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <libuvc/libuvc.h>
#include <turbojpeg.h>
#include <thread>
#include <chrono>
#include <future>

namespace PupilCamera
{
  enum class PupilCameraResults
  {
    STREAM_NOT_RUNNING = -3,
    CAMERA_NOT_FOUND = -2,
    FAILED = -1,    
    SUCCESS = 1
  };

  /*
  handles frame processing
  */
  struct Frame
  {
  public:
    Frame(int _height = 480, int _width = 640, int _mode = 1);

    ~Frame();
    
    //these are the functions that process the frames into the desired format
    int processFrame(uvc_frame_t frame);

    //a bunch of functions that are implemented in pupil, these are mostly placeholders
    int getWidth();

    int getHeight();

    int getID();

    //this function is defined as legacy
    PupilCameraResults getLastFrame(cv::Mat &img);

  private:

    int mJPEG2YUV(uvc_frame_t frame);
    int YUV2BGR();
    int YUV2Gray();


    //variables:
    tjhandle handleTH;
    long unsigned int bufferYUV;
    unsigned char *YUVData;
    int subsamp, width, height;
    int mode; //determines if this camera ready BGR or grayscale images, should be 1 for EGT cameras

    int id;

    cv::Mat lastFrame; //potentially useful
    cv::Mat currentFrame;
    std::mutex accessReadWrite;
  };


  /*
    handles camera processing
  */
  struct Camera
  {
  public:
    Camera();
    ~Camera();
    //must be executed at creation
    int init(); 
    PupilCameraResults openCamera(uvc_device_t* device);

    int startStream(int _height = 480, int _width = 640, int _fps = 120, int _channels = 1, float _bandwidth_factor = 2.0);
    PupilCameraResults stopStream();
    PupilCameraResults getLastFrame(cv::Mat &img);
    bool isOpen();
  private:
    
    uvc_device_handle_t *devh;
    uvc_device_t* dev;
    uvc_stream_ctrl_t ctrl;
    uvc_device_descriptor_t *desc;
    uvc_stream_handle_t *strmh;
    uvc_frame *uvc_frame;

    int grabFrames();
    
    int sleepDuration;
    std::future<int>  frameGrabber;
    bool killThread;

    Frame *frame;
  };

  //this class handles the processing of the libuvc devices
  //finds all available cameras
  //opens the desired camera, closes cameras, ...
  //is the inter
  class Camera_Manager
  {
    public:
      Camera_Manager();
      ~Camera_Manager();
      int init();
      int update(std::vector<std::string> &cameras);
      int getList(std::vector<std::string> &cameras);

      PupilCameraResults openCamera(int id);
      PupilCameraResults startStream(int id, int height_ = 480, int width_ = 640, int fps_ = 120, int channels_ = 1, float bandwidth_factor_ = 1.3);
      PupilCameraResults grabFrame(int id, cv::Mat& image);
      PupilCameraResults stopSream(int id);

      PupilCameraResults openCamera(std::string uid);
    private:
      int retrieveCameraList();
      uvc_context_t *ctx;
      uvc_device_t **dev_list;
      uvc_device_t *dev;
      uvc_device_descriptor_t *desc;
      
      std::vector<std::string> foundCameras;
      std::vector<std::pair<int, Camera*>> openCameras; //array or vector?
  };

  

};

#endif //_FRAMEGRABBER_H_
