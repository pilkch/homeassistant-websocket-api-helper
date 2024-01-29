#include <cerrno>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

namespace util {

// From: https://stackoverflow.com/a/1157217/1074390
int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}

std::string GetDateYYYYMMDD()
{
  std::time_t t = std::time(0);   // get time now
  std::tm* now = std::localtime(&t);
  std::ostringstream o;
  o<<std::setw(4)<<std::setfill('0')<<(now->tm_year + 1900)<<std::setw(2)<<(now->tm_mon + 1)<<std::setw(2)<<now->tm_mday;
  return o.str();
}

std::string GetHomeFolder()
{
  const char* szHomeFolder = getenv("HOME");
  if (szHomeFolder != nullptr) return szHomeFolder;

  struct passwd* pPasswd = getpwuid(getuid());
  if (pPasswd != nullptr) return pPasswd->pw_dir;

  return "";
}

std::string GetConfigFolder(std::string_view sApplicationNameLower)
{
  const std::string sHomeFolder = GetHomeFolder();
  if (sHomeFolder.empty()) return "";

  return sHomeFolder + "/.config/" + std::string(sApplicationNameLower);
}

bool TestFileExists(const std::string& sFilePath)
{
  struct stat s;
  return (stat(sFilePath.c_str(), &s) >= 0);
}

size_t GetFileSizeBytes(const std::string& sFilePath)
{
  struct stat s;
  if (stat(sFilePath.c_str(), &s) < 0) return 0;

  return s.st_size;
}

bool ReadFileIntoString(const std::string& sFilePath, size_t nMaxFileSizeBytes, std::string& contents)
{
  if (!TestFileExists(sFilePath)) {
    std::cerr<<"File \""<<sFilePath<<"\" not found"<<std::endl;
    return false;
  }

  const size_t nFileSizeBytes = GetFileSizeBytes(sFilePath);
  if (nFileSizeBytes == 0) {
    std::cerr<<"Empty file \""<<sFilePath<<"\""<<std::endl;
    return false;
  } else if (nFileSizeBytes > nMaxFileSizeBytes) {
    std::cerr<<"File \""<<sFilePath<<"\" is too large"<<std::endl;
    return false;
  }

  std::ifstream f(sFilePath);

  contents.reserve(nFileSizeBytes);

  contents.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

  return true;
}

}
