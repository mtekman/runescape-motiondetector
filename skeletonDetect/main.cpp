#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void usage(){
    cerr << "\nusage: skeletonDetect <first.jpg> <second.jpg> <kernel_size> [--debug]" << endl;
    cerr << "\tPrint X,Y coordinate of largest detected motion between two images for a given rectangular kernel " << endl;
    exit(-1);
}


int main(int argc, char ** argv)
{

    Mat early, later;
    int mask = 10;
    bool debug=false;

    if (argc<4 || argc>5) usage();

    if (argc >= 4){
        early = imread(argv[1]);
        later=  imread(argv[2]);
        mask = atoi(argv[3]);
        if (argc==5) debug = true;
    }

    Mat fgmask;

    //Background Subtract
/*    BackgroundSubtractorMOG fgbg;
    Mat kernel1 = cv::getStructuringElement(MORPH_RECT, Size(mask,mask));
    Mat kernel2 = cv::getStructuringElement(MORPH_RECT, Size(mask/2,mask/2));

    fgbg(later,fgmask);fgbg(early,fgmask);

    if (debug){
        imshow("Frame",fgmask);
        waitKey(0);
    }

    Mat tmp;
    erode(fgmask, tmp, kernel2);
    dilate(tmp,fgmask,kernel2);
*/

    // OR... simple subtraction, preserve colors allows us to filter by color
    fgmask = later - early;

    if (debug){
        imshow("Frame",fgmask);
        waitKey(0);
    }

    // REMOVE PLAYER
    // Black out center of screen
    int row_rad = fgmask.rows/2,     col_rad = fgmask.cols/2;
    int block_size_row = row_rad/4,  block_size_col = col_rad/6;
    int offset_X = 50,               offset_Y = -50;


    for (int y= offset_Y + row_rad - block_size_row; y< row_rad + block_size_row; y++)
    {
        for (int x= offset_X + col_rad - block_size_col; x < col_rad + block_size_col; x++)
        {
            Vec3b &pixels = fgmask.at<Vec3b>(y,x);
            pixels.val[0] = pixels.val[1] = pixels.val[2] = uchar(0);
        }
    }

    // POSTERIZE
    fgmask /=100;
    fgmask *=200;

    if (debug){
        imshow("Frame",fgmask);
        waitKey(0);
    }



    // set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
    SimpleBlobDetector::Params params;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
//    params.minDistBetweenBlobs = 5.0f;
/*    uchar pixels[3] = {100,100,100}; //RGB
    params.blobColor = *pixels;*/
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 5.0f;
    params.maxArea = 1000.0f;

    cv::Ptr<cv::FeatureDetector> blob_detector = new cv::SimpleBlobDetector(params);
    blob_detector->create("SimpleBlob");

    vector<cv::KeyPoint> keypoints;
    blob_detector->detect(fgmask, keypoints);

    //largest_blob
    float max_diam=-1;
    KeyPoint max;

    Mat overall;
    if (debug) overall=early+later;

    for (int i=0; i<keypoints.size(); i++){
        KeyPoint &kp = keypoints[i];
        float diam = kp.size;

        // YELLOW TEST
        //If not yellow: RED GREEN
        Vec3b &pix = fgmask.at<Vec3b>(kp.pt.y,kp.pt.x);

        float yellow_ratio = (float)(pix.val[2])/(float)(pix.val[1]);
        yellow_ratio *= (yellow_ratio>0)?1:-1;

        //If RG channels related, and G value greater than B
        if ((yellow_ratio > 0.8) && (pix.val[1] > pix.val[0])){
            continue;//Skip yellow
        }


        if (debug){
            cerr << "P" << i << " [" << (int)(kp.pt.x) << "," << (int)(kp.pt.y) << "] rad --> " << diam << endl;

            int rad = kp.size;

            for (int i=-rad; i < rad; i++){
                for (int j=-rad; j < rad; j++){
                    Vec3b &pixval1 = overall.at<Vec3b>(kp.pt.y+j,kp.pt.x+i);
                    Vec3b &pixval2 = fgmask.at<Vec3b>(kp.pt.y+j,kp.pt.x+i);
                    pixval1.val[2] = pixval2.val[2]  = uchar(255);
                    pixval1.val[1] = pixval1.val[0] = uchar(0);
                    pixval2.val[1] = pixval2.val[0] = uchar(0);
                }
            }
        }

        if (diam > max_diam){
            max_diam = diam;
            max = kp;
        }


    }

    if(debug){
        //color max GREEN
        int rad = max.size;
        for (int i=-rad; i < rad; i++){
            for (int j=-rad; j < rad; j++){
                Vec3b &pixval2 = fgmask.at<Vec3b>(max.pt.y+j,max.pt.x+i);
                pixval2.val[1]  = uchar(255);
                pixval2.val[2] = pixval2.val[0] = uchar(0);
            }
        }

        imshow("Frame",fgmask);
        waitKey(0);
    }

    //Largest:
    cout << (int)(max.pt.x) << " " << (int)(max.pt.y) << endl;
}

