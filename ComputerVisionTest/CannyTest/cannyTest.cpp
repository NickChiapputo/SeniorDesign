#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int main( int argc, char ** argv )
{
	// Ensure a file is given to the program
	if( argc != 2 )
	{
		printf( "Usage: ./displayImage <image_path>\n" );
		exit( -1 );
	}

	// Keep repeating process while user presses 'R' key
	int key = 114;
	while( key == 114 )
	{
		// Create matrix to hold image for each step
		Mat image, gray, gaussian, canny, cannyOverlay;


		// Create an image and read from the given source
		image = imread( argv[ 1 ], 1 );

		// Ensure image was properly loaded (i.e., source is valid)
		if( !image.data )
		{
			printf( "No image data.\n" );
			exit( -1 );
		}


		// Convert to grayscale
		cvtColor( image, gray, COLOR_BGR2GRAY );


		// Gaussian Blurring with user defined Gaussian kernel
		int kernel = 5;

		cout << "Gaussian Blur:\n" << "Kernel (odd): ";
		cin >> kernel;

		GaussianBlur( gray, gaussian, Size( kernel, kernel ), 0 );


		// Canny image detection
		int low_threshold = 100;
		int high_threshold = 150;
		int apertureSize = 3;

		cout << "\nCanny:\n" << "Low Threshold: ";
		cin >> low_threshold;
		cout << "High Threshold: ";
		cin >> high_threshold;
		cout << "Aperture Size (3-7): ";
		cin >> apertureSize;

		Canny( gaussian, canny, low_threshold, high_threshold, apertureSize );


		// Overlay canny on source image
		cannyOverlay = Scalar::all( 0 );		
		// image.copyTo( cannyOverlay, canny );								// Copy image source relative to canny output. One option of overlayed output
		
		// Second option for overlayed output. Blending by adding weighted values
		cannyOverlay = canny.clone();										// Copy canny image
		cvtColor( cannyOverlay, cannyOverlay, COLOR_GRAY2BGR );				// Convert canny image to BGR
		addWeighted( image, 0.4, cannyOverlay, 1.0, 0.0, cannyOverlay );	// Blend source image with canny image


		// Create window to display image in
		namedWindow( "Display", WINDOW_AUTOSIZE );
		imshow( "Display", cannyOverlay );


		// Wait for a keystroke in the window, then exit
		while( ( key = waitKey( 0 ) ) < 59 && key > 48 )
		{
			if( key == 49 )			// Key press = '1'
			{
				cout << "Change Display: Original\nPress 'R' to repeat.\n\n";
				imshow( "Display", image );
			}
			else if( key == 50 )	// Key press = '2'
			{
				cout << "Change Display: Gray Scale\nPress 'R' to repeat.\n\n";
				imshow( "Display", gray );
			}
			else if( key == 51 )	// Key press = '3'
			{
				cout << "Change Display: Gaussian Blur\nPress 'R' to repeat.\n\n";
				imshow( "Display", gaussian );
			}
			else if( key == 52 )	// Key press = '4'
			{
				cout << "Change Display: Canny\nPress 'R' to repeat.\n\n";
				imshow( "Display", canny );
			}
		}

		destroyWindow( "Display" );
	}

	return 0;
}