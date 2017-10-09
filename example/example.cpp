/*
example file for use of the pupil cam interface

Copyright

Copyright
Alexander Plopski
NAIST Interactive Media Design Laboratory
12.01.2017
*/

#include <pupilcam/FrameGrabber.hpp>

const int exposure_max = (int)((1.f/3) / 0.0001);

int main(int argc, char** argv)
{
  PupilCamera::Camera_Manager *manager = new PupilCamera::Camera_Manager();
  manager->init();
  std::vector<std::string> cameras;
  manager->update(cameras);
  std::cout << "number of cameras:" << cameras.size() << "\n";
  for (int i = 0; i < cameras.size(); ++i)
  {
    PupilCamera::PupilCameraResults status = manager->openCamera(i);
    if (status != PupilCamera::PupilCameraResults::SUCCESS)
    {
      std::cout << "failed to open camera"<<i<<"\n";
    }
    else
    {
      manager->startStream(i, 480, 640, 120, 3);
    }
  }
  
  int key = 0;

  cv::Mat tmp2;
  cv::RotatedRect tmp;

  std::vector<int> exposure;
  exposure.resize(cameras.size(), exposure_max);

  for (int i = 0; i < cameras.size(); ++i)
  {
    cv::namedWindow(std::to_string(i));
    cv::createTrackbar("exposure time", std::to_string(i), &exposure[i], exposure_max);
  }
  while (key != 'n')
  {
    for (int i = 0; i < cameras.size(); ++i)
    {
      manager->grabFrame(i, tmp2);
	  if (!tmp2.empty())
	  {
		  cv::imshow(std::to_string(i), tmp2);
	  }
    }
	key = cv::waitKey(10);
  }
  //stop stream for camera 1
  manager->stopStream(0);
  manager->openCamera(0);
  manager->startStream(0, 240, 320, 120, 1);
  key = 0;
  float duration = 0.0001f;
  int dir = 1;
  while (key != 'n')
  {
    for (int i = 0; i < cameras.size(); ++i)
    {
      if (dir)
      {
        PupilCamera::PupilCameraResults status = manager->setExposureTime(i, exposure[i] * 0.0001f);
        if (status != PupilCamera::PupilCameraResults::SUCCESS)
        {
          printf("error!\n");
        }
      }
      manager->grabFrame(i, tmp2);
      cv::imshow(std::to_string(i), tmp2);
      std::cout << duration << "\n";
    }
	  key = cv::waitKey(10);
    if (key == 'u')
    {
      manager->resetAutoExposure(0);
      exposure[0] = (int)((1.f / 60) / 0.0001);
    }
  }
  delete manager;
  return 0;
}