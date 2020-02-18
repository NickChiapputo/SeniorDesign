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


	// Create matrix to hold image for each step
	Mat image, gray, gaussian, canny, hough, linesHP, houghP;


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


	// Gaussian Blurring with 5x5 Gaussian kernel
	int kernel = 3;
	GaussianBlur( gray, gaussian, Size( kernel, kernel ), 0 );


	// Canny image detection
	int low_threshold = 5;
	int high_threshold = 150;
	int apertureSize = 3;
	Canny( image, canny, low_threshold, high_threshold, apertureSize );


	// Change canny to BGR image
	// Mat cannyBGR;
	cvtColor( canny, hough, COLOR_GRAY2BGR );


	// Standard Hough Line detection
	vector<Vec2f> lines;			// Vector that stores parameters (r, theta) of detected lines
	double rho = 1;					// The resolution of the parameter r in pixels.
	double theta = CV_PI / 180;		// The resolution of the paramter theta in radians. CV_PI / 180 = 1 degree
	int threshold = 150;			// The minimum number of intersections to detect a line
	double srn = 0;					// For multi-scale Hough transform. Divisor for the distance resolution rho. Classical Hough transform = 0
	double stn = 0;					// For multi-scale Hough transform. Divisor for the distance resolution theta. Classical Hough transform = 0
	Mat line_image = image * 0;

	// HoughLines( canny, lines, rho, theta, threshold, srn, stn );
	HoughLines( canny, lines, 1, theta, threshold, srn, stn );

	// Draw Hough lines
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[ i ][ 0 ];
		float theta = lines[ i ][ 1 ];

		Point pt1, pt2;

		double a = cos( theta );
		double b = sin( theta );

		double x0 = a * rho;
		double y0 = b * rho;

		pt1.x = cvRound( x0 + 1000 * ( -b ) );
		pt1.y = cvRound( y0 + 1000 * (  a ) );

		pt2.x = cvRound( x0 - 1000 * ( -b ) );
		pt2.y = cvRound( y0 - 1000 * (  a ) );

		line( hough, pt1, pt2, Scalar( 0, 0, 255 ), 3, LINE_AA );
	} 


	// Probabilistic Hough Line Transform
	houghP = image.clone();
	vector<Vec4i> linesP;
	double rhoP = 1;
	double thetaP = 1 * CV_PI / 180;
	int thresholdP = 150;
	double minLineLength = 5;
	double maxLineGap = 500;
	HoughLinesP( canny, linesP, rhoP, thetaP, thresholdP, minLineLength, maxLineGap );

	// Draw Hough Probabilistic Lines
	Scalar color = Scalar( 0, 0, 255 );
	for( size_t i = 0; i < linesP.size(); i++ )
	{
		Vec4i l = linesP[ i ];
		if( abs( l[ 0 ] - l[ 2 ] ) < 20 )
			line( houghP, Point( l[ 0 ], l[ 1 ] ), Point( l[ 2 ], l[ 3 ] ), color, 3, LINE_AA );
	}

	// Create window to display image in
	namedWindow( "Display", WINDOW_AUTOSIZE );
	imshow( "Display", image );


	// Wait for a keystroke in the window, then exit
	int key;
	while( ( key = waitKey( 0 ) ) < 59 && key > 48 )
	{
		if( key == 49 )			// Key press = '1'
		{
			cout << "Change Display: Original\n";
			imshow( "Display", image );
		}
		else if( key == 50 )	// Key press = '2'
		{
			cout << "Change Display: Gray Scale\n";
			imshow( "Display", gray );
		}
		else if( key == 51 )	// Key press = '3'
		{
			cout << "Change Display: Gaussian Blur\n";
			imshow( "Display", gaussian );
		}
		else if( key == 52 )	// Key press = '4'
		{
			cout << "Change Display: Canny\n";
			imshow( "Display", canny );
		}
		else if( key == 53 )	// Key press = '5'
		{
			cout << "Change Display: Hough Lines\n";
			imshow( "Display", hough );
		}
		else if( key == 54 )	// Key press = '6'
		{
			cout << "Change Display: Probabilistic Hough Lines\n";
			imshow( "Display", houghP );
		}
	}

	// Save image file
	// imwrite( "output.jpg", houghP );

	return 0;
}