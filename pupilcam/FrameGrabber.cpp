/*
this file is reimplimintation of the pyuvc scripts by Pupil-Labs 
https://github.com/pupil-labs/pyuvc

handles camera opening/closing in windows for libusbK
handles camera frame grabs - return openCV frames

Copyright
Alexander Plopski
NAIST Interactive Media Design Laboratory
12.01.2017
*/

#include "FrameGrabber.hpp"

using namespace PupilCamera;

Camera::Camera()
{
  strmh = NULL;
  desc = NULL;
  devh = NULL;
  uvc_frame = NULL;
}

int Camera::init()
{
  strmh = NULL;
  desc = NULL;
  devh = NULL;
  uvc_frame = NULL;
  return 1;
}

Camera::~Camera()
{
  if (strmh != NULL && !killThread)
  {
    stopStream();
  }
  if (devh!=NULL)
  {
    uvc_close(devh);
    devh = NULL;
  }
  uvc_unref_device(dev);
  dev = NULL;
  //if (uvc_frame !=NULL)
  //{
  //  //uvc_free_frame(uvc_frame); //it crashes if I try to kill the frame...
  //  uvc_frame = NULL;
  //}
}

PupilCameraResults Camera::openCamera(uvc_device_t* device)
{
  dev = device;
  uvc_ref_device(dev);
  uvc_error_t status = uvc_get_device_descriptor(dev, &desc);
  if (status != UVC_SUCCESS)
  {
    uvc_free_device_descriptor(desc);
    uvc_perror(status, "failed to retrieve device descriptor");
    return PupilCameraResults::FAILED;
  }
  status = uvc_open(dev, &devh);
  //uvc_print_diag(devh, stderr);
  if (status != UVC_SUCCESS)
  {
    uvc_perror(status, "failed to open camera");
    uvc_unref_device(dev);
    uvc_free_device_descriptor(desc);
    return PupilCameraResults::FAILED;
  }
  uvc_free_device_descriptor(desc);
  
  return PupilCameraResults::SUCCESS;
}

bool
Camera::isOpen()
{
  return (devh != NULL);
}

PupilCameraResults
Camera::setExposure(float duration)
{
  if (!isOpen())
  {
    printf("no stream");
    return PupilCameraResults::STREAM_NOT_RUNNING;
  }
  uvc_error_t result = uvc_set_ae_mode(devh, 1);//manual
  if (result != UVC_SUCCESS)
  {
    printf("error setting mode %d\n", result);
    return PupilCameraResults::FAILED;
  }
  result = uvc_set_ae_priority(devh, 0);
  if (result != UVC_SUCCESS)
  {
    printf("error setting priority %d\n", result);
    return PupilCameraResults::FAILED;
  }
  uint dur;
  result = uvc_get_exposure_abs(devh, &dur, UVC_GET_CUR);
  printf("duration1: %d\n", dur);
  result = uvc_set_exposure_abs(devh, (uint)(duration/0.0001));
  if (result != UVC_SUCCESS)
  {
    uvc_perror(result, "error setting time");
    result = uvc_set_exposure_abs(devh, (uint)(dur));
    return PupilCameraResults::FAILED;
  }
  return PupilCameraResults::SUCCESS;
}

PupilCameraResults
Camera::resetAutoExposure()
{
  if (!isOpen())
  {
    return PupilCameraResults::STREAM_NOT_RUNNING;  
  }
  uvc_error_t result = uvc_set_ae_mode(devh, 1);
  if (result != UVC_SUCCESS)
  {
    printf("failed to set exposure");
    return PupilCameraResults::FAILED;
  }
  result = uvc_set_ae_priority(devh, 0);
  uint dur;
  result = uvc_get_exposure_abs(devh, &dur, UVC_GET_DEF);
  result = uvc_set_exposure_abs(devh, dur);
  printf("duration: %d\n", dur);

  return PupilCameraResults::SUCCESS;
}

PupilCameraResults
Camera::getLastFrame(cv::Mat &img)
{
  if (frame == NULL)
  {
    return PupilCameraResults::STREAM_NOT_RUNNING;
  }
  return frame->getLastFrame(img);
}


int
Camera::startStream(int _height, int _width, int _fps, int _channels, float _bandwidth_factor)
{
  if ((_channels != 3) && (_channels != 1))
  {
    printf("incorrect number of channels, must be 1 for grayscale and 3 for bgr");
    return -1;
  }
  if (strmh != NULL)
  {
    stopStream();
  }
  uvc_error_t status = uvc_get_stream_ctrl_format_size(devh, &ctrl,
                               UVC_FRAME_FORMAT_COMPRESSED, _width, _height, _fps);
  if (status != UVC_SUCCESS)
  {
    uvc_perror(status, "Can't get stream control");
      return -1;
  }
  uvc_print_stream_ctrl(&ctrl, stderr);
  fflush(stderr);
  status = uvc_stream_open_ctrl(devh, &strmh, &ctrl);
  if (status != UVC_SUCCESS)
  {
    uvc_perror(status, "failed to open stream ctrl\n");
    strmh = NULL;
    return -1;
  }
  status = uvc_stream_start(strmh, NULL, NULL, _bandwidth_factor, 0);
  if (status != UVC_SUCCESS)
  {
    uvc_perror(status, "Can't start isochronous stream \n");
    strmh = NULL;
    return -1;
  }
  sleepDuration = 1000.0 / _fps;
  frame = new Frame(_height, _width, _channels);
  killThread = false;
  auto func = std::bind(&Camera::grabFrames, this);
  
  frameGrabber = std::async(std::launch::async, 
                            &Camera::grabFrames, this);
  return 1;
}


int
Camera::grabFrames()
{
  while (!killThread)
  {
    uvc_error_t status;
    for (int i = 0; i < 3; ++i)
    {
      status = uvc_stream_get_frame(strmh, &uvc_frame, 0);
      if (status == UVC_SUCCESS)
      {
        break;
      }
    }
    if (status != UVC_SUCCESS)
    {
      printf("could not grab frame\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }
    if (uvc_frame == NULL)
    {
      printf("frame is empty\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }
    //grab frame
    frame->processFrame(*uvc_frame);
    //sleep until next frame is available
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
  }
  std::cout << "exiting thread" << std::endl;
  return 1;
}

PupilCameraResults Camera::stopStream()
{
  killThread = true;
  std::future_status statusThread = frameGrabber.wait_for(std::chrono::milliseconds(0));
  while (statusThread != std::future_status::ready)
  {
   statusThread = frameGrabber.wait_for(std::chrono::milliseconds(0));
  }
  
  uvc_error_t status = uvc_stream_stop(strmh);
  if (status != UVC_SUCCESS)
  {
    for (int i = 0; i < 100; ++i)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      status = uvc_stream_stop(strmh);
      if (status == UVC_SUCCESS || status == UVC_ERROR_INVALID_PARAM)
      {
        break;
      }
    }
    if (status != UVC_SUCCESS && status != UVC_ERROR_INVALID_PARAM)
    {
      uvc_perror(status, "failed to stop stream; continue anyway \n");
//    return -1;
    }
  }
  uvc_stream_close(strmh);
  printf("Log: stream closed");
  strmh = NULL;
  delete frame;
  return PupilCameraResults::SUCCESS;
}

Camera_Manager::Camera_Manager()
{
}

Camera_Manager::~Camera_Manager()
{
  for (int camera = 0; camera < openCameras.size(); ++camera)
  {
    delete openCameras[camera].second;
  }
  if (dev_list != NULL)
  {
    uvc_free_device_list(dev_list, 1);
  }
  if (ctx != NULL)
  {
    uvc_exit(ctx);
    ctx = NULL;
  }
}

int
Camera_Manager::init()
{
  if (uvc_init(&ctx, NULL) != UVC_SUCCESS)
  {
    printf("failed to initialize context\n");
    return -1;
  }
}

int
Camera_Manager::getList(std::vector<std::string> &cameras)
{
  cameras = foundCameras;
  return 1;
}

int
Camera_Manager::update(std::vector<std::string> &cameras)
{
  int result = retrieveCameraList();
  cameras = foundCameras;
  return result;
}

int
Camera_Manager::retrieveCameraList()
{
  printf("retrieving\n");
  foundCameras.clear();
  openCameras.clear();
  if (ctx == NULL)
  {
    printf("initializing\n");
    if (init() == -1)
    {
      printf("failed to initialize context\ in updaten");
      return -1;
    }
  }

  if (uvc_get_device_list(ctx, &dev_list) == -1)
  {
    printf("could not get devices list.\n");
    return -1;
  }
  int id = 0;
  while (dev_list[id] != NULL)
  {
    dev = dev_list[id];
    uvc_error_t error = uvc_get_device_descriptor(dev, &desc);
    if ( error != UVC_SUCCESS)
    {
      uvc_free_device_descriptor(desc);
      uvc_perror(error, "failed to retrieve device descriptor");
      return -1;
    }
    if (desc->product == NULL)
    {
      uvc_free_device_descriptor(desc);
      id++;
      continue;
    }
    int16_t device_address = uvc_get_device_address(dev);
    int16_t bus_number = uvc_get_bus_number(dev);

    std::string uid = std::to_string(bus_number) + ":" + std::to_string(device_address);
    foundCameras.push_back(uid);
    openCameras.push_back(std::pair<int, Camera*>(id, new Camera()));
    uvc_free_device_descriptor(desc);
    id++;
    printf("id: %d\n", id);
  }
  
  std::cout << "number pupil cameras: " << foundCameras.size() << "\n";
  return 1;
}


PupilCameraResults
Camera_Manager::openCamera(int id)
{
  if (id > foundCameras.size())
  {
    return PupilCameraResults::CAMERA_NOT_FOUND;
  }
  if (openCameras[id].second->isOpen())
  {
    return PupilCameraResults::SUCCESS;
  }
  return openCameras[id].second->openCamera(dev_list[openCameras[id].first]);
}


PupilCameraResults
Camera_Manager::openCamera(std::string uid)
{
  for (int id = 0; id < foundCameras.size(); ++id)
  {
    if (uid.compare(foundCameras[id]) == 0)
    {
        return openCamera(id);
    }
  }
  return PupilCameraResults::CAMERA_NOT_FOUND;
}

PupilCameraResults
Camera_Manager::startStream(int id, int height_, int width_, int fps_, int channels_, float bandwidth_factor_)
{
  if (id > foundCameras.size())
  {
    return PupilCameraResults::CAMERA_NOT_FOUND;
  }
  if (!openCameras[id].second->isOpen())
  {
    if (openCameras[id].second->openCamera(dev_list[openCameras[id].first]) != PupilCameraResults::SUCCESS)
    {
      return PupilCameraResults::STREAM_NOT_RUNNING;
    }
  }
  if (openCameras[id].second->startStream(height_, width_, fps_, channels_, bandwidth_factor_))
  {
    return PupilCameraResults::SUCCESS;
  }
  return PupilCameraResults::STREAM_NOT_RUNNING;
}

PupilCameraResults
Camera_Manager::grabFrame(int id, cv::Mat &image)
{
  if (!openCameras[id].second->isOpen())
  {
    printf("camera %d not open\n", id);
    return PupilCameraResults::STREAM_NOT_RUNNING;
  }
  return  openCameras[id].second->getLastFrame(image);
}


PupilCameraResults
Camera_Manager::stopStream(int id)
{
  return openCameras[id].second->stopStream();
}

PupilCameraResults
Camera_Manager::resetAutoExposure(int id)
{
  return openCameras[id].second->resetAutoExposure();
}

PupilCameraResults
Camera_Manager::setExposureTime(int id, float duration)
{
  if (duration < 0.0001)
  {
    printf("exposure time must be at least 0.0001s\n");
    return PupilCameraResults::FAILED;
  }
  return openCameras[id].second->setExposure(duration);
}












