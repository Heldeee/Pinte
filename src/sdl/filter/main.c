#include "filtre.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 0;

  char* path = argv[1];
  negative(path);
  mirror(path);
  grayscale(path);
  yellow(path);
  rose(path);
  cyan(path);
  blackandwhite(path);
  rotationr(path);
  rotationl(path);
  
  return 1;
}
