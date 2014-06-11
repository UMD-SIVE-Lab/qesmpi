/*
 *  Created by Pete Willemsen on 10/25/12.
 *  Copyright 2012 Department of Computer Science, University of Minnesota-Duluth. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "util/png++/png.hpp"
#include "util/handleGraphicsArgs.h"
#include "util/Vector3D.h"

using namespace sivelab;

void clearImage(png::image< png::rgb_pixel > &im)
{
  int w = im.get_width();
  for (unsigned int idx=0; idx<im.get_height()*w; ++idx)
    {
      size_t x = idx % w;
      size_t y = static_cast<size_t>( floor(idx / static_cast<float>(w)) );
	      
      im[y][x] = png::rgb_pixel(255, 255, 255);
    }  
}

void splat(png::image< png::rgb_pixel > &im, int x, int y, const Vector3D &c)
{
  Vector3D val( (int)floor(c[0]), (int)floor(c[1]), (int)floor(c[2]) );

  int sR = 20;
  int w = im.get_width();

  int minY = ((y - sR) < 0) ? 0 : y - sR;
  int minX = ((x - sR) < 0) ? 0 : x - sR;

  int maxY = ((y + sR) > w) ? w : y + sR;
  int maxX = ((x + sR) > w) ? w : x + sR;
  
  for (size_t j = minY; j < maxY; ++j) {
    for (size_t i = minX; i < maxX; ++i) {
      
      float r = sqrt( (i-x)*(i-x) + (j-y)*(j-y) );
      if (r <= sR)
	im[j][i] = png::rgb_pixel(val[0], val[1], val[2]);
    }
  }  
  im[y][x] = png::rgb_pixel(128, 128, 128);
}


int main(int argc, char *argv[])
{
  GraphicsArgs args;
  args.process(argc, argv);

  int w = args.width, h = args.width;
  png::image< png::rgb_pixel > imData( w, h );

  std::ifstream fishEyeFile;
  fishEyeFile.open( args.inputFileName.c_str() );

  std::ostringstream fileName;

  int prevPatchID = -1, patchID = -1;
  float val;
  
  float dirx, diry, dirz;

  while (!fishEyeFile.eof())
    {
      char tmpChar;
      fishEyeFile >> patchID >> val >> tmpChar >> dirx >> tmpChar >> diry >> tmpChar >> dirz >> tmpChar;

      if (prevPatchID == -1)
	{
	  prevPatchID = patchID;
	  fileName << "fishEye_" << prevPatchID << ".png";

	  std::cout << patchID << std::endl;

	  clearImage(imData);
	}
      else if (prevPatchID != patchID)
	{
	  std::cout << patchID << std::endl;

	  // new patch id in file, close old image, get new image ready
	  imData.write( fileName.str().c_str() );
	  fileName.str("");

	  prevPatchID = patchID;
	  fileName << "fishEye_" << prevPatchID << ".png";

	  std::cout << "\tfilename = " << fileName.str() << std::endl;

	  clearImage(imData);
	}
      
      int x = w/2 + dirx * w/2, 
	y = w/2 + diry * w/2;

      Vector3D intensity = Vector3D(0, 191, 255) * val;
      splat(imData, x, y, intensity);
    }
  imData.write( fileName.str().c_str() );

  fishEyeFile.close();

  exit(EXIT_SUCCESS);
}
