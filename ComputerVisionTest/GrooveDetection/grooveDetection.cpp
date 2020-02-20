#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

void cannyEdgeDetection( Mat*, Mat*, Mat*, Mat*, int, int, int, double );
void standardHoughLineDetection( Mat*, Mat*, double, double, int, double, double, Scalar, int, int );
void probabilisticHoughLineDetection( Mat*, Mat*, vector<Vec4i>*, vector<Vec4i>*, double, double, int, double, double, Scalar, Scalar, int, int );
void probabilisticHoughLineGrouping( Mat*, vector<Vec4i>*, vector<Vec4i>*, Scalar, int, int );
void sortLinesIncreasingTopX( vector<Vec4i>* );
void drawROI( Mat*, vector<Vec4i>*, Scalar, double, int );

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


	/*********************/
	/*					 */
	/* GAUSSIAN BLURRING */
	/*					 */
	/*********************/
	int kernel = 3;
	GaussianBlur( gray, gaussian, Size( kernel, kernel ), 0 );



	/*************************/
	/*						 */
	/* CANNY IMAGE DETECTION */
	/*						 */
	/*************************/
	int low_threshold = 10;
	int high_threshold = 150;
	int apertureSize = 3;
	double cannyOverlayAlpha = 0.7;

	cannyEdgeDetection( &canny, &cannyOverlay, &image, &gaussian, low_threshold, high_threshold, apertureSize, cannyOverlayAlpha );



	/************************/
	/*						*/
	/* HOUGH LINE DETECTION */
	/*						*/
	/************************/
	// Change canny to BGR image
	cvtColor( canny, hough, COLOR_GRAY2BGR );


	// Standard Hough Line detection
	hough = cannyOverlay.clone();					// Set output image as canny overlay. Lines are drawn on top of this image

	Scalar houghLineColor = Scalar( 0, 0, 255 );	// Color of Hough lines on output image
	int houghLineThickness = 2;						// Pixel width of Hough lines
	int lineStyle = LINE_AA;

	double rho = 1;									// The resolution of the parameter r in pixels.
	double theta = CV_PI / 180;						// The resolution of the paramter theta in radians. CV_PI / 180 = 1 degree
	int threshold = 150;							// The minimum number of intersections to detect a line
	double srn = 0;									// For multi-scale Hough transform. Divisor for the distance resolution rho. Classical Hough transform = 0
	double stn = 0;									// For multi-scale Hough transform. Divisor for the distance resolution theta. Classical Hough transform = 0

	standardHoughLineDetection( &hough, &canny, rho, theta, threshold, srn, stn, houghLineColor, houghLineThickness, lineStyle );



	/**************************************/
	/*						 			  */
	/* PROBABILISTIC HOUGH LINE DETECTION */
	/*						 			  */
	/**************************************/
	// Hough line parameters
	houghP = cannyOverlay.clone();
	vector<Vec4i> linesP;
	vector<Vec4i> selectedPLines;
	double rhoP = 1;										// Check in small rho increments
	double thetaP = 1 * CV_PI / 180;						// Check in small theta increments
	int thresholdP = 100;									// Number of points needed to detect lines. Gets rid of sipes and block borders
	double minLineLength = 0.5 * cannyOverlay.rows;			// Only check for long lines (>= 90% of height)
	double maxLineGap = 0.15 * cannyOverlay.cols;			// Allow large gaps between lines. This overcomes the issue of blocks disrupting the grooves

	// Line style parameters
	Scalar houghPLineColor = Scalar( 0, 0, 255 );
	Scalar houghPSelectedLineColor = Scalar( 0, 255, 0 );
	int houghPLineThickness = 2; 
	int houghPLineStyle = LINE_AA;
	
	probabilisticHoughLineDetection( &houghP, &canny, &linesP, &selectedPLines, rhoP, thetaP, thresholdP, minLineLength, maxLineGap, houghPLineColor, houghPSelectedLineColor, houghPLineThickness, houghPLineStyle );



	/***********************************/
	/*						 		   */
	/* GROUP PROBABILISTIC HOUGH LINES */
	/*						 		   */
	/***********************************/
	grouped = cannyOverlay.clone();

	vector<Vec4i> averagedLines;

	Scalar groupedLineColor = Scalar( 0, 0, 255 );
	int groupedLineThickness = 2;
	int groupedLineStyle = LINE_AA;

	probabilisticHoughLineGrouping( &grouped, &selectedPLines, &averagedLines, groupedLineColor, groupedLineThickness, groupedLineStyle );



	/**********************/
	/*			 		  */
	/* SORT GROUPED LINES */
	/*					  */
	/**********************/
	sortLinesIncreasingTopX( &averagedLines );

	cout << "Sorted List of Lines: \n";
	for( size_t i = 0; i < averagedLines.size(); i++ )
	{
		Vec4i l = averagedLines[ i ];
		cout << "( " << l[ 0 ] << ", " << l[ 1 ] << " ) -> ( " << l[ 2 ] << ", " << l[ 3 ] << " )\n";
	}


	// Welcome to the groove zone
	grooveZone = image.clone();

	Scalar roiColor = Scalar( 119, 221, 119 );
	double roiAlpha = 0.6;
	int grooveZoneLineStyle = LINE_AA;

	drawROI( &grooveZone, &averagedLines, roiColor, roiAlpha, grooveZoneLineStyle );


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
	// imwrite( "output.jpg", grooveZone );

	return 0;
}

void cannyEdgeDetection( Mat * canny, Mat * cannyOverlay, Mat * src, Mat * gaussian, int low_threshold, int high_threshold, int aperture, double overlayAlpha )
{
	// Canny image detection
	Canny( *gaussian, *canny, low_threshold, high_threshold, aperture );


	// Overlay canny on source image
	*cannyOverlay = (*canny).clone();										// Copy canny image
	cvtColor( *cannyOverlay, *cannyOverlay, COLOR_GRAY2BGR );				// Convert canny image to BGR

	addWeighted( *src, overlayAlpha, *cannyOverlay, 1.0, 0.0, *cannyOverlay );	// Blend source image with canny image
}

void standardHoughLineDetection( Mat * dst, Mat * src, double rho, double theta, int threshold, double srn, double stn, Scalar lineColor, int lineThickness, int lineStyle )
{
	// Vector that stores parameters (r, theta) of detected lines
	vector<Vec2f> lines;			

	// Standard Hough Line detection. Set of output lines is stored in 'lines'
	HoughLines( *src, lines, rho, theta, threshold, srn, stn );

	// Draw Hough lines on output image
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

		line( *dst, pt1, pt2, lineColor, lineThickness, lineStyle );
	} 
}

void probabilisticHoughLineDetection( Mat * dst, Mat * src, vector<Vec4i> * lines, vector<Vec4i> * selectedLines, double rho, double theta, int threshold, double minLineLength, double maxLineGap, Scalar lineColor, Scalar selectedLineColor, int lineThickness, int lineStyle )
{
	// Probabilistic Hough Line Transform
	HoughLinesP( *src, *lines, rho, theta, threshold, minLineLength, maxLineGap );


	// Draw Hough Probabilistic Lines
	double averageTheta = 0;
	for( size_t i = 0; i < (*lines).size(); i++ )
	{
		Vec4i l = (*lines)[ i ];

		// Calculate angle compared to bottom of window
		double hypotenuseLength = sqrt( ( abs( l[ 0 ] - l[ 2 ] ) ) * ( abs( l[ 0 ] - l[ 2 ] ) ) + ( abs( l[ 1 ] - l[ 3 ] ) ) * ( abs( l[ 1 ] - l[ 3 ] ) ) );
		averageTheta += asin( abs( l[ 3 ] - l[ 1 ] ) / hypotenuseLength ) * 180 / ( CV_PI * (*lines).size() );

		// Only check for vertical lines whose bottom and top X values are within 20 of each other
		if( abs( l[ 0 ] - l[ 2 ] ) < 20 )
		{
			(*selectedLines).push_back( Vec4i( l[ 0 ], l[ 1 ], l[ 2 ], l[ 3 ] ) );

			cout << "Push Back: ( " << l[ 0 ] << ", " << l[ 1 ] << " ) -> ( " << l[ 2 ] << ", " << l[ 3 ] << " )\n";

			line( *dst, Point( l[ 0 ], l[ 1 ] ), Point( l[ 2 ], l[ 3 ] ), lineColor, lineThickness, LINE_AA );
		}
		else
		{
			line( *dst, Point( l[ 0 ], l[ 1 ] ), Point( l[ 2 ], l[ 3 ] ), selectedLineColor, lineThickness - 1, lineStyle );
		}
	}
	cout << "Average Theta: " << averageTheta << "\n\n";
}

void probabilisticHoughLineGrouping( Mat * dst, vector<Vec4i> * inLines, vector<Vec4i> * outLines, Scalar lineColor, int lineThickness, int lineStyle )
{
	// Find groups of houghP lines and average them together
	for( size_t i = 0; i < (*inLines).size(); i++ )
	{
		Vec4i currentLine = (*inLines)[ i ];
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
		for( size_t j = i + 1; j < (*inLines).size(); j++ )
		{
			Vec4i compareLine = (*inLines)[ j ];
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
				(*inLines).erase( (*inLines).begin() + j );


				// Decrement iterator
				// Next index of the list would be skipped after removal of previous line
				j--;
			}
		}

		if( numLines != 0 )
		{
			cout << "Average Line: \n";
			cout << avgLine[ 0 ] << " / " << numLines << " = ";
			avgLine[ 0 ] = avgLine[ 0 ] / numLines;
			cout << avgLine[ 0 ] << "\n" << avgLine[ 1 ] << " / " << numLines << " = ";
			avgLine[ 1 ] = avgLine[ 1 ] / numLines;
			cout << avgLine[ 1 ] << "\n" << avgLine[ 2 ] << " / " << numLines << " = ";
			avgLine[ 2 ] = avgLine[ 2 ] / numLines;
			cout << avgLine[ 2 ] << "\n" << avgLine[ 3 ] << " / " << numLines << " = ";
			avgLine[ 3 ] = avgLine[ 3 ] / numLines;
			cout << avgLine[ 3 ] << "\n";

			(*outLines).push_back( avgLine );

			cout << "( " << avgLine[ 0 ] << ", " << avgLine[ 1 ] << " ) -> ( " << avgLine[ 2 ] << ", " << avgLine[ 3 ] << " )\n\n";

			line( *dst, Point( avgLine[ 0 ], avgLine[ 1 ] ), Point( avgLine[ 2 ], avgLine[ 3 ] ), lineColor, lineThickness, lineStyle );
		}
	}
}

void sortLinesIncreasingTopX( vector<Vec4i>* lines )
{
	// Insertion Sort
	for( size_t i = 1; i < (*lines).size(); i++ )
	{
		Vec4i current = (*lines)[ i ];

		// Move values from 0 to i - 1 forward if greater than current value
		size_t j = i - 1;
		while( j >= 0 && (*lines)[ j ][ 0 ] > current[ 0 ] )
		{
			// Swap
			(*lines)[ j + 1 ] = (*lines)[ j ];
			j--;
		}
		(*lines)[ j + 1 ] = current;
	}
}

void drawROI( Mat * dst, vector<Vec4i> * lines, Scalar roiColor, double roiAlpha, int lineStyle )
{
	// Draw rectangles where grooves are estimated to be
	cout << "\n";
	for( size_t i = 0; i < (*lines).size() - 1; i+= 2 )
	{
		Vec4i line0 = (*lines)[ i ];
		Vec4i line1 = (*lines)[ i + 1];

		Mat dstCpy = (*dst).clone();

		// Draw filled polygon on copy
		int numPoints = 4;									// Four points on the polygon
		vector<Point> tmp;									// Create vector of points to connect (the polygon is drawn in order of points added)
		tmp.push_back( Point( line0[ 0 ], line0[ 1 ] ) );	// 		Select top point of line 1
		tmp.push_back( Point( line1[ 0 ], line1[ 1 ] ) );	// 		Connect to top point of line 2
		tmp.push_back( Point( line1[ 2 ], line1[ 3 ] ) );	// 		Connect to bottom point of line 2
		tmp.push_back( Point( line0[ 2 ], line0[ 3 ] ) );	// 		Connect to bottom point of line 1
		const Point * polyPoints[ 1 ] = { &tmp[ 0 ] };		// Create pointer to first element of the point list
		fillPoly( dstCpy, polyPoints, &numPoints, 1, roiColor, lineStyle );

		// Weight the image with ROI drawn with original image
		double alpha = 0.6;
		addWeighted( dstCpy, roiAlpha, *dst, 1.0 - roiAlpha, 0.0, *dst );
	}
}
