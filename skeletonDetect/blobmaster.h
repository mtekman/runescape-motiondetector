class SetBlobParams{
public:
    SimpleBlobDetector::Params params;

    SetBlobParams(){
        params.filterByInertia = false;
        params.filterByConvexity = false;
        params.filterByColor = false;
        params.minDistBetweenBlobs = 1.0f;
        /*    uchar pixels[3] = {100,100,100}; //RGB
        params.blobColor = *pixels;*/
        params.filterByCircularity = false;
        params.filterByArea = true;
        params.minArea = 5.0f;
        params.maxArea = 1000.0f;
    }
};



class BlobMaster{
public:
    map<float,KeyPoint> sorted_sizes; //Sorts blobs on radius key automatically (asc)

    BlobMaster(Mat &fgmask)
    {

        SetBlobParams *pm = new SetBlobParams;

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
