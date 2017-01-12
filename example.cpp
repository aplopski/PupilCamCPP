/*
example file for use of the pupil cam interface

Copyright

Copyright
Alexander Plopski
NAIST Interactive Media Design Laboratory
12.01.2017
*/

#include <pupilcam\FrameGrabber.hpp>

int main(int argc, char** argv)
{
  PupilCamera::Camera_Manager *manager = new PupilCamera::Camera_Manager();

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
      manager->startStream(i);
    }
  }
  
  int key = 0;

  cv::Mat tmp2;
  cv::RotatedRect tmp;
  while (key != 'n')
  {
    for (int i = 0; i < cameras.size(); ++i)
    {
      manager->grabFrame(i, tmp2);
      cv::imshow(std::to_string(i), tmp2);
    }
	key = cv::waitKey(10);
  }
  //stop stream for camera 1
  manager->stopSream(0);
  manager->openCamera(0);
  manager->startStream(0, 240, 320, 120, 1);
  key = 0;
  while (key != 'n')
  {
    for (int i = 0; i < cameras.size(); ++i)
    {
      manager->grabFrame(i, tmp2);
      cv::imshow(std::to_string(i), tmp2);
    }
	key = cv::waitKey(10);
  }
  delete manager;
  return 0;
}