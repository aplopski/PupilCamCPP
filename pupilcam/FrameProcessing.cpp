/*
implements all processing of the frames from the captured camera stream

Copyright
Alexander Plopski
NAIST Interactive Media Design Laboratory
12.01.2017
*/
#include "FrameGrabber.hpp"
using namespace PupilCamera;


Frame::Frame(int _height, int _width, int _mode) :
  bufferYUV(1),
  subsamp(0),
  width(_width),
  height(_height),
  mode(_mode),
  id(0)
{
  YUVData = new unsigned char[bufferYUV];
  handleTH = tjInitDecompress();
  if (mode == 1)
  {
    lastFrame = cv::Mat::zeros(height, width, CV_8UC1);
    currentFrame = cv::Mat::zeros(height, width, CV_8UC1);
  }
  else
  {
    lastFrame = cv::Mat::zeros(height, width, CV_8UC3);
    currentFrame = cv::Mat::zeros(height, width, CV_8UC3);
  }
};

Frame::~Frame()
{
  delete[] YUVData;
}

int
Frame::getWidth()
{
  return width;
}

int
Frame::getHeight()
{
  return height;
}

int
Frame::getID()
{
  return id;
}

//this function is defined as legacy
PupilCameraResults
Frame::getLastFrame(cv::Mat &img)
{
  accessReadWrite.lock();
  lastFrame.copyTo(img); //using copyTo here to prevent unintentional editing of stored data
  accessReadWrite.unlock();
  return PupilCameraResults::SUCCESS;
}


int Frame::processFrame(uvc_frame_t frame)
{
  //TODO: should this data be processed differently: copy the frame data, then process copied data?
  int result = mJPEG2YUV(frame);
  if (result == -1)
  {
    return result;
  }
  if (mode == 1)
  {
    result = YUV2Gray();
  }
  else
  {
    result = YUV2BGR();
  }
  if (result == 1)
  {
    id = frame.sequence;
  }
  return result;
}

int
Frame::mJPEG2YUV(uvc_frame_t frame)
{
  int j_width, j_height, j_subsamp;
  int result = tjDecompressHeader2(handleTH, (unsigned char *)frame.data,
    frame.data_bytes, &j_width, &j_height, &j_subsamp);
  if (result == -1)
  {
    printf("Turbojpeg could not read jpeg header: %s", tjGetErrorStr());
    //HACK SOLUTION
    j_subsamp = TJSAMP_422;
    j_width = width;
    j_height = height;

    return result;
  }
  subsamp = j_subsamp;
  long unsigned int buf_size = tjBufSizeYUV(width, height, subsamp);
  if (buf_size != bufferYUV)
  {
    delete[] YUVData;
    YUVData = new unsigned char[buf_size];
  }
  result = tjDecompressToYUV(handleTH,
    (unsigned char *)frame.data,
    frame.data_bytes,
    &YUVData[0],
    0);
  if (result == -1)
  {
    printf("Turbojpeg could not read jpeg header: %s", tjGetErrorStr());
    return result;
  }
  return 1;
}

int
Frame::YUV2BGR()
{
  
  if ((currentFrame.channels() != mode) || (currentFrame.rows != height) || (currentFrame.cols != width) || (currentFrame.depth() != CV_8U)) //TODO: unnecessary checks?
  {
    currentFrame = cv::Mat::zeros(height, width, CV_8UC3);
  }
  int result = tjDecodeYUV(handleTH, &YUVData[0], 4, subsamp,
    &currentFrame.data[0], width, 0, height, TJPF_BGR, 0);
  if (result == -1)
  {
    printf("Turbojpeg could not convert to BGR: %s", tjGetErrorStr());
    return result;
  }
  accessReadWrite.lock();
  currentFrame.copyTo(lastFrame);
  accessReadWrite.unlock();
  return 1;
}

int Frame::YUV2Gray()
{
  if ((currentFrame.channels() != mode) || (currentFrame.rows != height) || (currentFrame.cols != width) || (currentFrame.depth() != CV_8U)) //TODO: unnecessary checks?
  {
    currentFrame = cv::Mat::zeros(height, width, CV_8UC1);
  }
  //assume that the image width is always a multiple of 4! (no padding)
  memcpy(currentFrame.data, YUVData, width*height); //
  accessReadWrite.lock();
  currentFrame.copyTo(lastFrame);
  accessReadWrite.unlock();
  return 1;
}