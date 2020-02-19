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
	Mat image, gray, gaussian, canny, cannyOverlay, hough, linesHP, houghP, grouped, grooveZone;


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
	int low_threshold = 10;
	int high_threshold = 150;
	int apertureSize = 3;
	Canny( gaussian, canny, low_threshold, high_threshold, apertureSize );


	// Overlay canny on source image
	cannyOverlay = Scalar::all( 0 );		
	// image.copyTo( cannyOverlay, canny );								// Copy image source relative to canny output. One option of overlayed output
	
	// Second option for overlayed output. Blending by adding weighted values
	cannyOverlay = canny.clone();										// Copy canny image
	cvtColor( cannyOverlay, cannyOverlay, COLOR_GRAY2BGR );				// Convert canny image to BGR

	double cannyOverlayAlpha = 0.7;
	addWeighted( image, cannyOverlayAlpha, cannyOverlay, 1.0, 0.0, cannyOverlay );	// Blend source image with canny image


	// Change canny to BGR image
	cvtColor( canny, hough, COLOR_GRAY2BGR );


	// Standard Hough Line detection
	hough = cannyOverlay.clone();
	vector<Vec2f> lines;			// Vector that stores parameters (r, theta) of detected lines
	double rho = 1;					// The resolution of the parameter r in pixels.
	double theta = CV_PI / 180;		// The resolution of the paramter theta in radians. CV_PI / 180 = 1 degree
	int threshold = 150;			// The minimum number of intersections to detect a line
	double srn = 0;					// For multi-scale Hough transform. Divisor for the distance resolution rho. Classical Hough transform = 0
	double stn = 0;					// For multi-scale Hough transform. Divisor for the distance resolution theta. Classical Hough transform = 0
	Mat line_image = image * 0;

	// HoughLines( canny, lines, rho, theta, threshold, srn, stn );
	HoughLines( canny, lines, rho, theta, threshold, srn, stn );

	// Draw Hough lines
	Scalar color = Scalar( 0, 0, 255 );
	int thickness = 2;
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

		line( hough, pt1, pt2, color, thickness, LINE_AA );
	} 


	// Probabilistic Hough Line Transform
	houghP = cannyOverlay.clone();
	vector<Vec4i> linesP;
	double rhoP = 1;										// Check in small rho increments
	double thetaP = 1 * CV_PI / 180;						// Check in small theta increments
	int thresholdP = 100;									// Number of points needed to detect lines. Gets rid of sipes and block borders
	double minLineLength = 0.5 * cannyOverlay.rows;			// Only check for long lines (>= 90% of height)
	double maxLineGap = 0.15 * cannyOverlay.cols;			// Allow large gaps between lines. This overcomes the issue of blocks disrupting the grooves
	
	HoughLinesP( canny, linesP, rhoP, thetaP, thresholdP, minLineLength, maxLineGap );

	double averageTheta = 0;

	vector<Vec4i> selectedPLines;

	// Draw Hough Probabilistic Lines
	for( size_t i = 0; i < linesP.size(); i++ )
	{
		Vec4i l = linesP[ i ];

		// Calculate angle compared to bottom of window
		double hypotenuseLength = sqrt( ( abs( l[ 0 ] - l[ 2 ] ) ) * ( abs( l[ 0 ] - l[ 2 ] ) ) + ( abs( l[ 1 ] - l[ 3 ] ) ) * ( abs( l[ 1 ] - l[ 3 ] ) ) );
		averageTheta += asin( abs( l[ 3 ] - l[ 1 ] ) / hypotenuseLength ) * 180 / ( CV_PI * linesP.size() );

		// Only check for vertical lines whose bottom and top X values are within 20 of each other
		if( abs( l[ 0 ] - l[ 2 ] ) < 20 )
		{
			selectedPLines.push_back( Vec4i( l[ 0 ], l[ 1 ], l[ 2 ], l[ 3 ] ) );

			cout << "Push Back: ( " << l[ 0 ] << ", " << l[ 1 ] << " ) -> ( " << l[ 2 ] << ", " << l[ 3 ] << "\n";

			line( houghP, Point( l[ 0 ], l[ 1 ] ), Point( l[ 2 ], l[ 3 ] ), color, thickness, LINE_AA );
		}
		// else
		// {
		// 	line( houghP, Point( l[ 0 ], l[ 1 ] ), Point( l[ 2 ], l[ 3 ] ), Scalar( 0, 255, 0 ), thickness - 1, LINE_AA );
		// }
	}
	cout << "Average Theta: " << averageTheta << "\n\n";


	// Find groups of houghP lines and average them together
	grouped = cannyOverlay.clone();
	vector<Vec4i> averagedLines;
	for( size_t i = 0; i < selectedPLines.size(); i++ )
	{
		Vec4i currentLine = selectedPLines[ i ];
		Vec4i avgLine = Vec4i( 0, 0, 0, 0 );

		// Determine which coordinate is the top coordinate
		// First coordinate of average line is the top coordinate
		avgLine[ 0 ] += currentLine[ 1 ] < currentLine[ 3 ] ? currentLine[ 0 ] : currentLine[ 2 ];
		avgLine[ 1 ] += currentLine[ 1 ] < currentLine[ 3 ] ? currentLine[ 1 ] : currentLine[ 3 ];
		avgLine[ 2 ] += currentLine[ 1 ] < currentLine[ 3 ] ? currentLine[ 2 ] : currentLine[ 0 ];
		avgLine[ 3 ] += currentLine[ 1 ] < currentLine[ 3 ] ? currentLine[ 3 ] : currentLine[ 1 ];

		cout << "( " << currentLine[ 0 ] << ", " << currentLine[ 1 ] << " ) -> ( " << currentLine[ 2 ] << ", " << currentLine[ 3 ] << " )\n";

		double currMedian = ( currentLine[ 0 ] + currentLine[ 2 ] ) / 2;

		int numLines = 1;
		for( size_t j = i + 1; j < selectedPLines.size(); j++ )
		{
			Vec4i compareLine = selectedPLines[ j ];
			double compareMedian = ( compareLine[ 0 ] + compareLine[ 2 ] ) / 2;

			// If center of lines is within 20 pixels
			if( abs( compareMedian - currMedian ) < 30 || abs( currentLine[ 0 ] - compareLine[ 0 ] ) < 30 || abs( currentLine[ 2 ] - compareLine[ 2 ] ) < 30 )
			{
				cout << "( " << compareLine[ 0 ] << ", " << compareLine[ 1 ] << " ) -> ( " << compareLine[ 2 ] << ", " << compareLine[ 3 ] << " )\n";

				numLines++;

				// Determine which coordinate is the top coordinate
				// First coordinate of average line is the top coordinate
				avgLine[ 0 ] += compareLine[ 1 ] < compareLine[ 3 ] ? compareLine[ 0 ] : compareLine[ 2 ];
				avgLine[ 1 ] += compareLine[ 1 ] < compareLine[ 3 ] ? compareLine[ 1 ] : compareLine[ 3 ];
				avgLine[ 2 ] += compareLine[ 1 ] < compareLine[ 3 ] ? compareLine[ 2 ] : compareLine[ 0 ];
				avgLine[ 3 ] += compareLine[ 1 ] < compareLine[ 3 ] ? compareLine[ 3 ] : compareLine[ 1 ];


				// Remove line from the selected lines list
				// It should not be considered in the averages of another grouping
				selectedPLines.erase( selectedPLines.begin() + j );


				// Decrement iterator
				// Next index of the list would be skipped after removal of previous line
				j--;
			}
		}

		if( numLines != 0 )
		{
			cout << avgLine[ 0 ] << " / " << numLines << " = ";
			avgLine[ 0 ] = avgLine[ 0 ] / numLines;
			cout << avgLine[ 0 ] << "\n" << avgLine[ 1 ] << " / " << numLines << " = ";
			avgLine[ 1 ] = avgLine[ 1 ] / numLines;
			cout << avgLine[ 1 ] << "\n" << avgLine[ 2 ] << " / " << numLines << " = ";
			avgLine[ 2 ] = avgLine[ 2 ] / numLines;
			cout << avgLine[ 2 ] << "\n" << avgLine[ 3 ] << " / " << numLines << " = ";
			avgLine[ 3 ] = avgLine[ 3 ] / numLines;
			cout << avgLine[ 3 ] << "\n";

			averagedLines.push_back( avgLine );

			cout << "( " << avgLine[ 0 ] << ", " << avgLine[ 1 ] << " ) -> ( " << avgLine[ 2 ] << ", " << avgLine[ 3 ] << " )\n\n";

			line( grouped, Point( avgLine[ 0 ], avgLine[ 1 ] ), Point( avgLine[ 2 ], avgLine[ 3 ] ), color, thickness, LINE_AA );
		}
	}


	// Sort lines by top left x
	for( size_t i = 1; i < averagedLines.size(); i++ )
	{
		Vec4i current = averagedLines[ i ];

		size_t j = i - 1;
		while( j >= 0 && averagedLines[ j ][ 0 ] > current[ 0 ] )
		{
			// Swap
			averagedLines[ j + 1 ] = averagedLines[ j ];
			j--;
		}
		averagedLines[ j + 1 ] = current;
	}

	cout << "Sorted List of Lines: \n";
	for( size_t i = 0; i < averagedLines.size(); i++ )
	{
		Vec4i l = averagedLines[ i ];
		cout << "( " << l[ 0 ] << ", " << l[ 1 ] << " ) -> ( " << l[ 2 ] << ", " << l[ 3 ] << " )\n";
	}


	// Welcome to the groove zone
	// grooveZone = canny.clone();										// Copy canny image
	// cvtColor( grooveZone, grooveZone, COLOR_GRAY2BGR );				// Convert canny image to BGR
	// addWeighted( image, 1.0, grooveZone, 1.0, 0.0, grooveZone );	// Blend source image with canny image (full color for both)
	grooveZone = grouped.clone();

	// Draw rectangles where grooves are estimated to be
	cout << "\n";
	for( size_t i = 0; i < averagedLines.size() - 1; i+= 2 )
	{
		Vec4i line0 = averagedLines[ i ];
		Vec4i line1 = averagedLines[ i + 1];

		// Create copy of grooveZone
		Mat grooveZoneCpy = grooveZone.clone();

		// Draw filled polygon on copy
		int numPoints = 4;									// Four points on the polygon
		Scalar rectColor = Scalar( 0, 255, 0 );				// Color of the polygons
		vector<Point> tmp;									// Create vector of points to connect (the polygon is drawn in order of points added)
		tmp.push_back( Point( line0[ 0 ], line0[ 1 ] ) );	// 		Select top point of line 1
		tmp.push_back( Point( line1[ 0 ], line1[ 1 ] ) );	// 		Connect to top point of line 2
		tmp.push_back( Point( line1[ 2 ], line1[ 3 ] ) );	// 		Connect to bottom point of line 2
		tmp.push_back( Point( line0[ 2 ], line0[ 3 ] ) );	// 		Connect to bottom point of line 1
		const Point * polyPoints[ 1 ] = { &tmp[ 0 ] };		// Create pointer to first element of the point list
		fillPoly( grooveZoneCpy, polyPoints, &numPoints, 1, rectColor, LINE_AA );

		// Weight the 
		double alpha = 0.4;
		addWeighted( grooveZoneCpy, alpha, grooveZone, 1.0 - alpha, 0.0, grooveZone );

		// addWeighted( color, alpha, roi, 1.0 - alpha, 0.0, roi );
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
			imshow( "Display", cannyOverlay );
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
		else if( key == 55 )	// Key press = '7'
		{
			cout << "Change Display: Probabilistic Hough Lines with grouped lines\n";
			imshow( "Display", grouped );
		}
		else if( key == 56 )	// Key press = '8'
		{
			cout << "Change Display: Groove detection zone\n";
			imshow( "Display", grooveZone );
		}
	}

	// Save image file
	imwrite( "output.jpg", grooveZone );

	return 0;
}