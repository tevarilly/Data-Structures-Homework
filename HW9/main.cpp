#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <math.h>

#include "image.h"
#include "priority_queue.h"

// ===================================================================================================
// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);
// ===================================================================================================



// ===================================================================================================
int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
	// EXTRA CREDIT: create other visualizations 
	std::cerr << "ERROR: Unknown visualization style" << std::endl;
	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }

  return 0;
}
// ===================================================================================================


// ===================================================================================================
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) 
{
    int w = input.Width();
    int h = input.Height();

    // return the maximum distance value
    double answer = 0;

    // loop over the pixels in the input image
    for (int i = 0; i < w; i++)  
    {
        for (int j = 0; j < h; j++) 
        {
            double closest = -1;      

            // loop over all other pixels in the input image
            for (int i2 = 0; i2 < w; i2++)  
            {
	            for (int j2 = 0; j2 < h; j2++) 
                {
	                const Color& c = input.GetPixel(i2,j2);   

	                // skip all pixels that are not black
	                if (!c.isBlack()) continue;

	                // calculate the distance between the two pixels
	                double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));

	                // store the closest distance to a black pixel
	                if (closest < 0 || distance < closest) 
                    {
	                    closest = distance;
	                }
	            }
            }

            assert (closest >= 0);
            answer = std::max(answer,closest);

            // save the data to the distance image
            DistancePixel& p = distance_image.GetPixel(i,j);
            p.setValue(closest);
        }
    }

  return answer;
}
// ===================================================================================================


// ===================================================================================================
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) 
{
    int w = input.Width();
    int h = input.Height();

    // Vector for storing the black pixel coordinates
    std::vector<std::pair<int,int > > black_pixels;

    // Return the maximum distance value
    double answer = 0;
      
    // Loop over the pixels in the input image
    for(int i=0; i<w; i++)  
    {
        for(int j=0; j<h; j++) 
        {
            const Color& c = input.GetPixel(i,j);   

            // If the pixel is black, add it to the vector
            if(c.isBlack()) 
            {
                std::pair<int,int> coords;
                coords = std::make_pair(i,j);

                black_pixels.push_back(coords);
            }
        }
    }

    // Loop over the pixels in the input image once more
    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            double closest = -1;

            // For each pixel, compare to the black pixels
            for(uint k=0; k<black_pixels.size(); ++k)
            {
                int i2 = black_pixels[k].first;
                int j2 = black_pixels[k].second;

                // Calculate the distance between the two pixels
                double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));

                // Store the closest distance to a black pixel
                if(closest < 0 || distance < closest) 
                {
                    closest = distance;
                }
            }

            assert (closest >= 0);
            answer = std::max(answer,closest);

            // Save the data to the distance image
            DistancePixel& p = distance_image.GetPixel(i,j);
            p.setValue(closest);
        }
    }
    
    return answer;
}
// ===================================================================================================


// ===================================================================================================
// A faster function which implements a priority queue to check all the neighbors of each pixel
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  
    int w = input.Width();
    int h = input.Height();

    //Define infinity
    double inf = std::numeric_limits<double>::infinity();

    // Vector for storing the black pixel types
    std::vector<DistancePixel*> black_pixels;

    // Loop over the pixels in the input image
    for(int i=0; i<w; i++)  
    {
        for(int j=0; j<h; j++) 
        {
            const Color& c = input.GetPixel(i,j);   

            // If the pixel is black
            if(c.isBlack()) 
            {
                // Make a new distance pixel object
                DistancePixel* one_pixel = &distance_image.GetPixel(i,j);

                // All black pixels are set to zero
                one_pixel->setX(i);
                one_pixel->setY(j);
                one_pixel->setValue(0);

                // Add it to the vector
                black_pixels.push_back(one_pixel);

                // Set the distance values in the output image
                DistancePixel& p = distance_image.GetPixel(i,j);
                p.setValue(0);    
            }

            // Set the distance values of all other pixels to infinity
            else
            {
                DistancePixel& p = distance_image.GetPixel(i,j);
                p.setX(i);
                p.setY(j);
                p.setValue(inf);
            }
        }
    }

    // Construct our priority queue out of the black pixels only (to start)
    DistancePixel_PriorityQueue priority_queue(black_pixels);

    // Return the maximum distance value
    double answer = 0;

    // While the priority queue is not empty
    while(!priority_queue.empty())
    {
        // Remove the first item in the queue and store it temporarily 
        const DistancePixel* temp = priority_queue.top();
        priority_queue.pop();
 
        // Look left
        if(temp->getX()-1 >= 0) 
        {
            DistancePixel* left = &distance_image.GetPixel(temp->getX()-1,temp->getY());

            // If the pixel to the left has a greater distance value than it should
            if(left->getValue() > temp->getValue()+1)
            {
                double doPush = left->getValue();

                // Modify the left value and re-add it to the queue
                left->setValue(temp->getValue()+1);
                if(doPush==inf)
                {
                    priority_queue.push(left);
                }   
            }
        }

        // Look right
        if(temp->getX()+1 < distance_image.Width()) 
        {
            DistancePixel* right = &distance_image.GetPixel(temp->getX()+1,temp->getY());

            // If the pixel to the right has a greater distance value than it should
            if(right->getValue() > temp->getValue()+1)
            {
                double doPush = right->getValue();

                // Modify the right value and re-add it to the queue
                right->setValue(temp->getValue()+1);
                if(doPush==inf)
                {
                    priority_queue.push(right);
                }
            }

        }

        // Look up
        if(temp->getY()-1 >= 0)
        {
            DistancePixel* up = &distance_image.GetPixel(temp->getX(),temp->getY()-1);

            // If the pixel above has a greater distance value than it should
            if(up->getValue() > temp->getValue()+1)
            {
                double doPush = up->getValue();

                // Modify the above value and re-add it to the queue
                up->setValue(temp->getValue()+1);
                if(doPush==inf)
                {
                    priority_queue.push(up);
                }
            }
        }

        // Look down
        if(temp->getY()+1 < distance_image.Height()) 
        {
            DistancePixel* down = &distance_image.GetPixel(temp->getX(),temp->getY()+1);

            // If the pixel below has a greater distance value than it should
            if(down->getValue() > temp->getValue()+1)
            {
                double doPush = down->getValue();

                // Modify the above value and re-add it to the queue
                down->setValue(temp->getValue()+1);
                if(doPush==inf)
                {
                    priority_queue.push(down);
                }
            }
        }

        // Look up-left diagonal
        if(temp->getY()-1 >= 0 && temp->getX()-1 >= 0) 
        {
            DistancePixel* up_left = &distance_image.GetPixel(temp->getX()-1,temp->getY()-1);

            // If the up-left pixel has a greater distance value than it should
            if(up_left->getValue() > temp->getValue()+sqrt(2))
            {
                double doPush = up_left->getValue();

                // Modify the up-left value and re-add it to the queue
                up_left->setValue(temp->getValue()+sqrt(2));
                
                if(doPush==inf)
                {
                    priority_queue.push(up_left);
                }
            }
        }

        // Look down-left diagonal
        if(temp->getY()+1 < distance_image.Height() && temp->getX()-1 >= 0)
        {
            DistancePixel* down_left = &distance_image.GetPixel(temp->getX()-1,temp->getY()+1);

            // If the down-left pixel has a greater distance value than it should
            if(down_left->getValue() > temp->getValue()+sqrt(2))
            {
                double doPush = down_left->getValue();

                // Modify the down-left value and re-add it to the queue
                down_left->setValue(temp->getValue()+sqrt(2));
                
                if(doPush==inf)
                {
                    priority_queue.push(down_left);
                }
            }
        }

        // Look up-right diagonal
        if(temp->getY()-1 >= 0 && temp->getX()+1 < distance_image.Width()) 
        {
            DistancePixel* up_right = &distance_image.GetPixel(temp->getX()+1,temp->getY()-1);

            // If the up_right pixel has a greater distance value than it should
            if(up_right->getValue() > temp->getValue()+sqrt(2))
            {
                double doPush = up_right->getValue();

                // Modify the up-right value and re-add it to the queue
                up_right->setValue(temp->getValue()+sqrt(2));
                
                if(doPush==inf)
                {
                    priority_queue.push(up_right);
                }
            }
        }

        // Look down-right diagonal
        if(temp->getY()+1 < distance_image.Height() && temp->getX()+1 < distance_image.Width()) 
        {
            DistancePixel* down_right = &distance_image.GetPixel(temp->getX()+1,temp->getY()+1);

            // If the down_right pixel has a greater distance value than it should
            if(down_right->getValue() > temp->getValue()+sqrt(2))
            {
                double doPush = down_right->getValue();
                
                // Modify the down-right value and re-add it to the queue
                down_right->setValue(temp->getValue()+sqrt(2));
                
                if(doPush==inf)
                {
                    priority_queue.push(down_right);
                }
            }
        }
    }

    // Loop over the pixels in the out image
    for(int i=0; i<w; i++)  
    {
        for(int j=0; j<h; j++) 
        {
            if(distance_image.GetPixel(i,j).getValue() > answer)
            {
                answer = distance_image.GetPixel(i,j).getValue();
            }
        }
    }

    return answer;
}
// ===================================================================================================


// ===================================================================================================
Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}
// ===================================================================================================


// ===================================================================================================
Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}
// ===================================================================================================
