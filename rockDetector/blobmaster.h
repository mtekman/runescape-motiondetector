
class RockParams{
public:
    SimpleBlobDetector::Params params;

    RockParams(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByColor = true;
        params.minDistBetweenBlobs = 3.0f;

        uchar pixels[3] = {122,101,73}; //RGB, brownish grey
        params.blobColor = *pixels;
        params.filterByCircularity = true;
        params.filterByArea = true;
        // Rock size
        params.minArea = 25.0f;
        params.maxArea = 30.0f;
    }
};


class TwinkleParams{
public:
    SimpleBlobDetector::Params params;

    TwinkleParams(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByColor = true;
        params.minDistBetweenBlobs = 1.0f;

        uchar pixels[3] = {250,250,250}; //RGB, white
        params.blobColor = *pixels;
        params.filterByCircularity = false;
        params.filterByArea = true;
        // Twinkle size
        params.minArea = 3.0f;
        params.maxArea = 5.0f;
    }
};


class BlobMaster{
public:
    map<float,KeyPoint> sorted_sizes; //Sorts blobs on radius key automatically (asc)

    BlobMaster(Mat &fgmask)
    {

        TwinkleParams *pm = new TwinkleParams;

        cv::Ptr<cv::FeatureDetector> blob_detector = new cv::SimpleBlobDetector(pm->params);
        blob_detector->create("SimpleBlob");

        vector<cv::KeyPoint> keypoints;
        blob_detector->detect(fgmask, keypoints);

        // Sort Blobs from biggest to smallest
        for (int k=0; k<keypoints.size(); k++)
        {
            KeyPoint &kp = keypoints[k];
            sorted_sizes[kp.size]=kp;
        }
    }
};
