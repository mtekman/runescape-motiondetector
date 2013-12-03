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


    Mat kernel1 = cv::getStructuringElement(MORPH_RECT, Size(mask,mask));
    Mat kernel2 = cv::getStructuringElement(MORPH_RECT, Size(mask/2,mask/2));

    BackgroundSubtractorMOG fgbg;

    Mat fgmask;
    fgbg(later,fgmask);
    fgbg(early,fgmask);

    if (debug){
        imshow("Frame",fgmask);
        waitKey(0);
    }

    Mat tmp;
    erode(fgmask, tmp, kernel2);
    dilate(tmp,fgmask,kernel2);

    if (debug){
        imshow("Frame",fgmask);
        waitKey(0);
    }



    // set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
    SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = 50.0f;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 20.0f;
    params.maxArea = 500.0f;
    // ... any other params you don't want default value

    // set up and create the detector using the parameters
    cv::Ptr<cv::FeatureDetector> blob_detector = new cv::SimpleBlobDetector(params);
    blob_detector->create("SimpleBlob");

    // detect!
    vector<cv::KeyPoint> keypoints;
    blob_detector->detect(fgmask, keypoints);

    // extract the x y coordinates of the keypoints:
    //largest_blob
    float max_diam=-1;
    KeyPoint max;

    Mat overall;
    if (debug) overall=early+later;

    for (int i=0; i<keypoints.size(); i++){
        KeyPoint &kp = keypoints[i];
        float diam = kp.size;

        if (debug){
            cerr << "P" << i << " [" << (int)(kp.pt.x) << "," << (int)(kp.pt.y) << "] rad --> " << diam << endl;

            int rad = kp.size;

            for (int i=-rad; i < rad; i++){
                for (int j=-rad; j < rad; j++){
                    Vec3b &pixval = overall.at<Vec3b>(kp.pt.y+j,kp.pt.x+i);
                    pixval.val[2] = uchar(255);
                    pixval.val[1] = pixval.val[0] = uchar(0);
                }
            }
        }

        if (diam > max_diam){
            max_diam = diam;
            max = kp;
        }
    }

    if(debug){
        imshow("Frame",overall);
        waitKey(0);
    }

    //Largest:
    cout << (int)(max.pt.x) << " " << (int)(max.pt.y) << endl;
}

